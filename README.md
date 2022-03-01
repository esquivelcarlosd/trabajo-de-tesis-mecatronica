# trabajo-de-tesis-mecatronica
Este código fue desarollado para controlar el microcontrolador del trabajo que me otorgó el grado de ingeniero mecatrónico cuyo nombre de tesis es "DETECTOR PARA UBICAR MATERIAL DE TUBERIAS Y VARILLAS EN EL INTERIOR DE UNA ESTRUCTURA

En términos generales, este trabajo permite al usuario detectar metales y no metales a través de un dispositivo con una pantalla táctil.

Por un lado, tenemos al Arduino Maestro que es el encargado de recibir la información proporcionada por los sensores y mostrar dichos datos en una pantalla táctil.
En segunda instancia, el Arduino esclavo es el encargado de transmitir la información proporcionada por la bobina detectora de metales ferrosos y no ferrosos mediante una conección I2C.
