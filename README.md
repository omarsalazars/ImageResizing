# Image Interpollation
Image interpollation using these methods:
* Nearest Neighbor
* Bilineal Lagrange Interpolation of multiple grades
* Average between 2 images

## Dependencies
* libjpeg
* libpng

## Run
```
git clone git@github.com:omarsalazars/BicubicInterpolation.git
cd BicubicInterpolation
make
./interpolation
```

## Results
#### Original Image
![Original Image](https://github.com/omarsalazars/BicubicInterpolation/blob/master/Imagen1/img.jpg)

#### Augmented Zone
![Augmented Zone](https://github.com/omarsalazars/BicubicInterpolation/blob/master/Imagen1/aux.png)

#### Nearest Neighbor
![Nearest Neighbor](https://github.com/omarsalazars/BicubicInterpolation/blob/master/Imagen1/nearest.png)

#### Average
![Average](https://github.com/omarsalazars/BicubicInterpolation/blob/master/Imagen1/promedio.png)

#### Bilineal Interpolation
![Bilineal Interpollation Grade 2](https://github.com/omarsalazars/BicubicInterpolation/blob/master/Imagen1/bilineal.png)

#### Bicubic Interpolation
![Bilineal Interpolation Grade 3](https://github.com/omarsalazars/BicubicInterpolation/blob/master/Imagen1/bicubica.png)

#### Bi-5 Interpolation
![Bilineal Interpolation Grade 5](https://github.com/omarsalazars/BicubicInterpolation/blob/master/Imagen1/quinta.png)

#### Bi-10 Interpolation
![Bilineal Interpolation Grade 10](https://github.com/omarsalazars/BicubicInterpolation/blob/master/Imagen1/decima.png)
