import cv2
from pyzbar.pyzbar import decode


cap = cv2.VideoCapture(0) # ここでエラーを吐いたら ls /dev/video* を実行してカメラを認識しているか確認

cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

while True:
    ret, frame = cap.read()
    # cv2.imshow("test", frame) # GUIが使える時だけ
    gray = cv2.cvtColor(frame, cv2.COLOR_RGBA2GRAY)
    data = decode(gray)
    for symbol in data:
        print(data[0][0].decode("utf-8", "ignore"))
        break
            
            
    
    if cv2.waitKey(1) == 27: # Escキーで終了(GUIがある場合のみ)
        break

cap.release()
#cv2.destroyAllWindows()