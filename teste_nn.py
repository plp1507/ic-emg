#! /bin/python3

import os
os.environ['TFF_CPP_MIN_LOG_LEVEL'] = '2'

import tensorflow as tf

from tensorflow import keras
from tensorflow.keras import layers
#mnist é o dataset pra reconhecimento de numeros
#from tensorflow.keras.datasets import mnist

#dataset sao os valores processados
(x_train, y_train), (x_test, y_test) = dataset.load_data()

#p/ normalizar os valores da leitura -> ampl de 0 a 1
(x_train) /= 4096

#rede neural sequencial:
model = keras.Sequential(
    [
    keras.Input(shape = 6),
    layers.Dense(64, activation = 'relu'),
    layers.Dense(32, activation = 'relu'),
    ]
)

#compilaçao do modelo
model.compile(

    #losses: diferença entre a resposta da rede neural e a resposta esperada
    #loss function = Binary Cross Entropy: usada quando o output pode estar em 2 estados
    loss = keras.losses.BinaryCrossentropy(),

    #lr -> learning rate; default é 0.001
    optimizer = keras.optimizers.Adam(),
    metrics = ["accuracy"],
)

model.fit(x_train, y_train, batch_size=32, epochs=5, verbose=2)
model.evaluate(x_test, y_test, batch_size=32, verbose=2)
