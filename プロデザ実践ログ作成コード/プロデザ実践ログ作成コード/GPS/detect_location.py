import csv

def get_locationdata():
    with open('./bus_locationdata.csv', encoding="Shift-JIS", newline='') as f:
        csvreader = csv.reader(f)
        location= [row for row in csvreader]  
    del location[0]
    return location


def detect_location(x,y):
    name="NULL"
    for i in range(len(location_data)):
        if float(location_data[i][1])<= float(x) <=float(location_data[i][2]) and float(location_data[i][4])<= float(y) <=float(location_data[i][3]):
            name=(location_data[i][0])
            break
    return name

location_data=get_locationdata()
print("15:42:19.0")
print("緯度経度： 36.53244535, 136.6089806")
print(" ")
print("                 ↓   プログラム実行          ")
print(" ")
print("位置情報: "+detect_location(36.53244535,136.6089806))#GPSから緯度経度入力
