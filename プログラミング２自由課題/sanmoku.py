
import random
Rps_Table   = {'グー': '1', 'チョキ': '2', 'パー': '3'}
Rps_Table_2 = {'1':"グー" , '2':'チョキ' , '3':'パー' }
gameBoard   = [0,1,2,3,4,5,6,7,8]
xs          = []

def Rps():
    while True:
        a=random.randint(1,3)
        b=input("グー,チョキ,パー,のどれかを入力してください : ")
        result = a - int(Rps_Table[b])
        print('cpu...'+Rps_Table_2[str(a)]+'！')
        if result == 1 or result == -2 :
            Rps_result=('win')
            return Rps_result
            break
        elif result == 0 :
            print('あいこです,もう1度じゃんけんしましょう。')
            print('')
        else:
            Rps_result=('Lose')
            return Rps_result
            break
    
def displayBoard():
    print('---------')
    for i in range(0,9):
        if i % 3 != 2 :
            print('|'+str(gameBoard[i])+'|',end = "")
        else:
            print('|'+str(gameBoard[i])+'|')
            print('---------')
def win():
    lines = [
      [0, 1, 2],
      [3, 4, 5],
      [6, 7, 8],
      [0, 3, 6],
      [1, 4, 7],
      [2, 5, 8],
      [0, 4, 8],
      [2, 4, 6],
    ]
    for i in range(0, 8):
        [a, b, c] = lines[i]
        if gameBoard[a] and gameBoard[a] == gameBoard[b] and gameBoard[a] == gameBoard[c]:
            return('win')


if  Rps() == ('Lose'):
    print('あなたの負けです。あなたが後攻でゲームを始めます。')   
    g=int(random.randint(0,8))
    gameBoard[g]=('\033[34m'+'×'+'\033[0m')
    xs.append(g)
    print('cpu...私は'+str(g)+'を選びました。')
else:
    print('あなたの勝ちです。あなたが先攻でゲームを始めます')
displayBoard()
while True:
    f=int(input("cpu...あなたのターンです0から８までの番号を選んでください。 : "))
    if f  not in  xs:
            gameBoard[f]=('\033[31m'+'○'+'\033[0m')
            xs.append(f)
            if win() == 'win' :
                displayBoard()
                print('あなたの勝ち')
                break
            if len(xs)==9 :
                displayBoard()
                print('引き分けですね')
                break
            while True:
                g=int(random.randint(0,8))
                if g not in  xs:
                    gameBoard[g]=('\033[34m'+'×'+'\033[0m')
                    xs.append(g)
                    displayBoard()
                    print('cpu...私は'+str(g)+'を選びました。')                                           
                    break
            if win() == 'win' :
                print('私の勝ち')
                break
            if len(xs)==9:
                displayBoard()
                print('引き分けですね')
                break
                        
    else:
        displayBoard()
        print('もうすでに埋まっています。もう一度選択してください。')

    
            
    

    



            
            
        
    


