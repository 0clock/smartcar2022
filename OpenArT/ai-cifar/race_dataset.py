import tensorflow as tf
from importer import DataStoreWrapper
import numpy as np
import tqdm

if __name__ =='__main__':
    class_names=[
        "cat","cow" ,"dog", "horse", "pig" ,
        "apple","banana","durian", "grape","orange",
        "bus", "plane", "sedan", "ship", "train"
        ]
    x_train =np.load("x_train.npy")
    y_train = np.load("y_train.npy")
    datastore =DataStoreWrapper(datastore='http://127.0.0.1:10812')
    datastore.create_project(project_name='race.deepview',path='.')
    image_shape=(64,64)
    print('\n上传训练集\n')
    for i in tqdm.tqdm(range(x_train.shape[0])):
        datastore.upload_image(x_train[i],class_names[y_train[i]],image_shape)