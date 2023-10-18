main:
	g++ -std=c++20 -o ImageResizing MathUtil.cpp DisplayedImage.cpp AugmentedImage.cpp main.cpp -lpthread -lX11 -ljpeg -lpng
	@echo "run ./ImageResizing"
