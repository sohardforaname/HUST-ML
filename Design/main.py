from face import face_rec
import os

def work():
    face = face_rec("D:\\dlib\\sp.dat", "D:\\dlib\\fr.dat", "D:\\dlib\\face", "D:\\dlib\\c.txt")

    str = "1"
    while 1:
        str = input()
        if str == "-e":
            break

        elif str == "-reg":
            print("input the path and name")
            new_path = input()
            new_can = input()
            face.append_img(new_path, new_can)

        elif str == "-rec":
            print("input the path")
            new_path = input()
            face.reco(new_path)

        else:
            print("Error command")

work()
os.system("pause")


