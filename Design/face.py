import dlib
import os
import glob
import numpy
from skimage import io

class face_rec:
    predictor_path = "D:\\dlib\\sp.dat"
    face_rec_model_path = "D:\\dlib\\fr.dat"
    faces_folder_path = "D:\\dlib\\face"

    def __init__(self, predictor_path, face_rec_model_path, face_folder_path, candidate_path):
        self.predictor_path = predictor_path
        self.face_rec_model_path = face_rec_model_path
        self.faces_folder_path = face_folder_path
        self.detector = dlib.get_frontal_face_detector()
        self.sp = dlib.shape_predictor(self.predictor_path)
        self.face_rec = dlib.face_recognition_model_v1(self.face_rec_model_path)
        self.descriptors = []
        self.candidate_path = candidate_path
        self.candidate = []

        for f in glob.glob(os.path.join(self.faces_folder_path, "*.jpg")):
            print("Processing file: {}".format(f))
            img = io.imread(f)
            dets = self.detector(img, 1)
            print("Number of faces detected: {}".format(len(dets)))

            for k, d in enumerate(dets):
                shape = self.sp(img, d)
                face_descriptor = self.face_rec.compute_face_descriptor(img, shape)
                arr = numpy.array(face_descriptor)
                self.descriptors.append(arr)

        with open(self.candidate_path, "r") as f:
            self.candidate = f.readlines()
            for i in range(0, len(self.candidate)):
                self.candidate[i] = self.candidate[i].rstrip('\n')

        f.close()

    def reco(self, img_path):
        img = io.imread(img_path)
        dets = self.detector(img, 1)
        dist = []

        for k, d in enumerate(dets):
            shape = self.sp(img, d)
            face_descriptor = self.face_rec.compute_face_descriptor(img, shape)
            d_test = numpy.array(face_descriptor)

            for i in self.descriptors:
                dist_ = numpy.linalg.norm(i - d_test)
                dist.append(dist_)

        c_d = dict(zip(self.candidate, dist))
        cd_sorted = sorted(c_d.items(), key = lambda d: d[1])

        if cd_sorted[0][1] > 0.4:
            print("Not found")
        else:
            print("The person is: ", cd_sorted[0][0])

        dlib.hit_enter_to_continue()

    def append_img(self, img_path, name):
        img = io.imread(img_path)
        dets = self.detector(img, 1)
        print("Number of faces detected: {}".format(len(dets)))

        for k, d in enumerate(dets):
            shape = self.sp(img, d)
            face_descriptor = self.face_rec.compute_face_descriptor(img, shape)
            arr = numpy.array(face_descriptor)
            self.descriptors.append(arr)

        io.imsave(self.faces_folder_path + '\\' + str(len(self.descriptors)) + ".jpg", img)

        self.candidate.append(name)
        for i in range(0, len(self.candidate)):
            self.candidate[i] = self.candidate[i] + ('\n')

        with open(self.candidate_path, "w") as f:
            f.writelines(self.candidate)

        f.close()
