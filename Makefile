main:
	g++ -std=c++11 -o interpolation MathUtil.cpp AugmentedImage.cpp main.cpp -lpthread -lX11 -ljpeg -lpng
	@echo "run ./interpolation"
