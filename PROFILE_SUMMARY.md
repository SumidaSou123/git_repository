# MPIマッチング実行 - プロファイリングまとめ

**概要**
- 実行日: 2026-05-21
- 実行コマンド（task-spooler経由）:
  - ts bash -c 'source /Users/ccadmin/Virtual/cv-tg/bin/activate && cd /Users/ccadmin/mnt/For_Cluster/CV-TG/code/mpi_panorama_matching && MODE=match mpiexec -n 2 python main.py --config config/frame000000_config.json'

**実施したこと**
- HDF5（pano特徴量）を作成し、`match` モードで MPI (mpiexec -n 2) 実行。
- `run_gpu_batch_matching()` に OpenCL イベントプロファイリング（`PROFILING_ENABLE` キュー、enqueue_copy と kernel の event.profile を利用）を追加。
- ワーカー側 (`worker_process_dynamic`) を修正して `gpu_profile` を受け取り、累積して終了時に出力するようにした。
- 結果ヒストグラムを保存: `CV-TG/img/frame_000000/feature_parallel_2stage_histogram.png`

**観察されたタイミング（今回の実行）**
- 総処理時間: 14.144 s
- Stage1 (粗いスキャン): 12.601 s
- ワーカープロファイル (Rank 1): I/O=0.000 s, GPU=12.592 s, Comm=0.020 s
- OpenCL 内部（イベント計測）: HtoD=0.000 s, Kernel=0.290 s, DtoH=0.000 s
- 総マッチ数（Stage1 合計）: 1501 matches
- 最良フレーム: Frame 109 = 938 matches

**ノード数 / プロセス構成**
- mpiexec 起動: `-n 2` → MPI size=2 → マスター(0) + ワーカー数=1
- 実際の並列ワーカー数: 1 ノード（ワーカープロセス数）

**主要なコードパス**
- マスター/ワーカー制御: `CV-TG/code/mpi_panorama_matching/main.py` ([ファイル](CV-TG/code/mpi_panorama_matching/main.py))
- GPUカーネル: `CV-TG/code/mpi_panorama_matching/sift_matcher.cl` ([ファイル](CV-TG/code/mpi_panorama_matching/sift_matcher.cl))
- 設定（実行用JSON）: `CV-TG/code/mpi_panorama_matching/config/frame000000_config.json` ([ファイル](CV-TG/code/mpi_panorama_matching/config/frame000000_config.json))
- 出力ヒストグラム: `CV-TG/img/frame_000000/feature_parallel_2stage_histogram.png` ([ファイル](CV-TG/img/frame_000000/feature_parallel_2stage_histogram.png))

**フィルター・設定値（ソース内の現在値）**
- `MIN_PANO_KEYPOINT_SIZE`: 8.0
- `MIN_KEYPOINT_SIZE`: 8.0
- `ENABLE_PANO_SIZE_FILTER`: True
- `ENABLE_SIZE_FILTER` (ユーザー画像): False
- `RATIO_THRESH`: 0.75
- `CHUNK_SIZE`: 5000
- `STAGE1_STEP`: 1
- `STAGE1_RANGE`: 2

（上記は `CV-TG/code/mpi_panorama_matching/main.py` のグローバル設定に基づく）

**問題点（現象）**
- ワーカーの `GPU` 時間（12.592 s）に対して、OpenCL の実際のカーネル実行時間は 0.290 s しかない。
- HtoD / DtoH がほぼ 0s と出ている一方で、ワーカー側で大きく時間が掛かっている。これは以下の可能性を示す:
  - 多数の小さなカーネル呼び出し／バッファ操作によりホスト側（キュー、同期、MPI呼び出し等）の待ち時間が累積している
  - `queue.finish()` や頻繁な同期が存在し、GPU の実行を逐次待っている
  - タスクチャンクが小さすぎてオーバーヘッドが支配的
  - マスター−ワーカー間の頻繁な要求応答（小バッチ送信）による待ち時間

**改善案（優先順）**
1. **チャンクサイズを増やす**（簡単かつ効果大）
   - `CHUNK_SIZE` を 5000 → 10000 や 20000 にして、カーネル起動回数を減らす。
   - 期待効果: ホストオーバーヘッド削減、GPU利用効率向上。

2. **バッチ化でまとめてカーネルを起動**
   - 1 回のカーネルで複数チャンク（または複数フレーム）を処理するようカーネル／呼び出しロジックを改修。
   - 期待効果: カーネル起動オーバーヘッドの削減。

3. **非同期ストリーミングとイベントの重畳**
   - `enqueue_copy` / `enqueue_nd_range_kernel` を非同期に呼び、`queue.finish()` を最小化してストリーム処理を行う。
   - 期待効果: データ転送と計算のオーバーラップ、ホスト待ち削減。ただし実装とデバッグのコストがある。

4. **マスター→ワーカーのバッチ送信を増やす**
   - `master_distribute_tasks()` の `batch_size` を 1 から大きくする（例: 5〜10）ことで通信頻度を下げる。
   - 期待効果: MPI 待ち時間・送受信回数の削減。

5. **詳細プロファイル出力**（診断用）
   - 各チャンクごとに `gpu_profile` を JSON に書き出し（`user_htod`/`pano_htod`/`kernel`/`dtoh`）、後で集計してヒートマップ化する。
   - 期待効果: どのチャンク／フレームがボトルネックかを特定できる。

6. **カーネル側最適化**
   - グローバル/ローカルサイズ、ローカルメモリ利用、メモリアクセスパターンを見直し、1呼び出し当たりの作業量を増やす。

**推奨アクション（短期）**
- まず `CHUNK_SIZE` を 2 倍〜4 倍にして再ベンチマーク。効果が大きければそれで運用。
- 効果が不十分なら 2) バッチ化 + 4) マスターのバッチ送信増 を実施。

**参考データ / 重要ログ抜粋**
- ワーカー GPU 内訳: `[Rank 1]   GPU breakdown: HtoD=0.000s Kernel=0.290s DtoH=0.000s`
- ワーカー合計: `[Rank 1]   I/O: 0.000s, GPU: 12.592s, Comm: 0.020s`
- ヒストグラムファイル: `CV-TG/img/frame_000000/feature_parallel_2stage_histogram.png`

---

必要なら、以下を代行します:
- 即座に `CHUNK_SIZE` を変更してベンチ（ts ジョブ）を再実行して比較レポート作成
- `master_distribute_tasks()` の初期 `batch_size` を増やすパッチ適用と比較
- 詳細チャンクごとの JSON プロファイリング出力を追加してボトルネックを可視化

どれを先に行いますか？
