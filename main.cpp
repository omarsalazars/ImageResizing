#define cimg_use_jpeg
#define cimg_use_png
#include "CImg.h"
#include "DisplayedImage.h"
#include "AugmentedImage.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace cimg_library;
using namespace std;

const char* instructions = R"(Usage: ./ImageResizing [options]

Options:
  - method [method]      : The selected resizing method to apply. See Resizing Methods section.
  - path [path]          : Specify the path to the image. Default: Image1/img.jpg.
  - initialX [value]     : Initial X coordinate of the expanded area. Default: 100.
  - initialY [value]     : Initial Y coordinate of the expanded area. Default: 100.
  - finalX [value]       : Final X coordinate of the expanded area. Default: 200.
  - finalY [value]       : Final Y coordinate of the expanded area. Default: 200.
  - incrementX [value]   : X axis increment of the image. Default: 4.
  - incrementY [value]   : Y axis increment of the image. Default: 4.
  - save [true|false]    : Save images. Default: false.
  - display [true|false] : Display images. Default true.
  - savePath [value]     : Path to save the images. If save=false, savePath will be ignored. savePath has to exist in the file system.

      InitialX/Y
        ----------------
        [              ]
        ----------------
                        FinalX/Y

  Resizing Methods:
  - NearestNeighbor       : Resize using the Nearest Neighbor method.
  - Bilinear -n [degree]  : Resize using Bilinear interpolation with the specified degree.
  - ImageExpansion        : Resize using the Image Expansion method.
  - All                   : Resize using NearestNeighbor, Biilineal interpolation grade 2 and 3, Image Expansion and Average of interpolation grade 2 and 3

Usage Examples:
  - Resize using Nearest Neighbor:
    ./ImageResizing -method NearestNeighbor -path image_path.jpg

  - Resize using Bilinear interpolation with degree 2:
    ./ImageResizing -method Bilinear -n 2 -path image_path.jpg

  - Resize using Image Expansion with specific coordinates:
    ./ImageResizing -method ImageExpansion -path image_path.jpg -initialX 10 -initialY 20 -finalX 100 -finalY 200
)";

CImg<unsigned char> originalImage;
AugmentedImage augmentedImage;

unordered_map<string, string> defaults = {
    {"method", "All"},
    {"path", "Image1/img.jpg"},
    {"initialX", "100"},
    {"initialY", "100"},
    {"finalX", "200"},
    {"finalY", "200"},
    {"incrementX", "4"},
    {"incrementY", "4"},
    {"save", "false"},
    {"display", "true"},
    {"savePath", "results/"}
};

unordered_map<string, string> options;
unordered_set<string> methods({ "Bilinear", "NearestNeighbor", "ImageExpansion", "All" });
using Function = function<void(AugmentedImage&)>;
unordered_map<string, Function> functionMap;

int initialX;
int initialY;
int finalX;
int finalY;
int incrementX;
int incrementY;

void die(const char* errorMessage){
    cerr << errorMessage << endl;
    cerr << instructions << endl;
    exit(1);
}

void validateArguments(){
    initialX = stoi(options["initialX"]);
    initialY = stoi(options["initialY"]);
    finalX = stoi(options["finalX"]);
    finalY = stoi(options["finalY"]);
    incrementX = stoi(options["incrementX"]);
    incrementY = stoi(options["incrementY"]);

    // Validate arguments are in range
    if(initialX<0 || initialY<0 || finalX<0 || finalY<0 || incrementX<0 || incrementY<0) 
        die("All arguments must be poisitive.");

    if(initialX > originalImage.width()) die("initialX must be shorter than max image width");
    if(initialY > originalImage.height()) die("initialY must be shorter than max image height");
    if(finalX > originalImage.width()) die("finalX must be shorter than max image width");
    if(finalY > originalImage.height()) die("finalY must be shorter than max image height");
    if(finalX < initialX) die("finalX must be shorter than initialX");
    if(finalY < initialY) die("finalY must be shorter than initialY");
}

void init(int argc, char* argv[])
{
    int size = argc;

    if(size >= 2){
        if(strcmp(argv[1], "-help") == 0){
            cout << instructions << endl;
            exit(0);
        }
    }

    const char* filename = options["path"].c_str();

    originalImage.assign(filename);

    validateArguments();

    augmentedImage = (AugmentedImage)originalImage.get_crop( initialX, initialY, finalX, finalY );
    augmentedImage.setIncrementX(incrementX);
    augmentedImage.setIncrementY(incrementY);

    cout<<endl;
    cout << "Width: " << originalImage.width() << endl;
    cout << "Height: " << originalImage.height() << endl;
    cout << "Spectrum: " << originalImage.spectrum() << endl;

    cout << "cropped Width: " << augmentedImage.width() << endl;
    cout << "cropped Height: " << augmentedImage.height() << endl;
    cout << "cropped Spectrum: " << augmentedImage.spectrum() << endl;
}

void parseArguments(int argc, char* argv[]){
    std::vector<std::string> args(argv, argv+argc);

    // Get arguments without '-' character
    for(int i=1; i<args.size(); i+=2){
        options[args[i].substr(1)] = args[i+1];
    }

    // Check arguments names. If argument is invalid, will be ignored.
    for(auto it = options.begin(); it != options.end(); it++){
        if(!defaults.contains(it->first)){
            cout<<"Option \""<<it->first<<"\" is invalid and will be ignored."<<endl;
        }
    }

    // Check if all values are defined in options. If not, use default values.
    for(auto it = defaults.begin(); it != defaults.end(); it++){
        if(!options.contains(it->first)){
            options[it->first] = it->second;
            cout<<it->first<<" not defined. Using default value "<<it->second<<endl;
        }
    }

    cout<<endl<<"Defined options:"<<endl;
    for(auto it = defaults.begin(); it!=defaults.end(); it++){
        cout<<it->first<<" = "<<options[it->first]<<endl;
    }

    if(methods.find(options["method"]) == methods.end()){
        cout<<"Method \""<<options["method"]<<"\" does not exists. Using default \"All\".";
        options["method"] = "All";
    }
}

void display(vector<DisplayedImage> images){
    CImgDisplay main_disp(originalImage, "Original");
    vector<CImgDisplay> displays;

    for(DisplayedImage displayedImage : images){
        displays.push_back(CImgDisplay(displayedImage.image, displayedImage.displayName.c_str()));
    }

    while (!main_disp.is_closed()){
        main_disp.wait();
    }
}

void save(string prefix, vector<DisplayedImage> images){
    for(DisplayedImage displayedImage : images){
        string imagePath = prefix+displayedImage.imageName;
        displayedImage.image.save(imagePath.c_str());
    }
}

int main(int argc, char* argv[])
{
    parseArguments(argc, argv);
    init(argc, argv);

    vector<DisplayedImage> images;
    images.push_back(DisplayedImage(augmentedImage, "augmented.png", "Augmented"));
    
    // Fill functionMap
    functionMap = {
        {"All", [&](AugmentedImage& img){
            images.push_back(img.EmptySpace());
            images.push_back(img.NearestNeighbor());
            images.push_back(img.NLagrangeInterpolation(2));
            images.push_back(img.NLagrangeInterpolation(3));
        }},
        {"ImageExpansion", [&](AugmentedImage& img){
            images.push_back(img.EmptySpace());
        }},
        {"NearestNeighbor", [&](AugmentedImage& img){
            images.push_back(img.NearestNeighbor());
        }},
        {"Bilinear", [&](AugmentedImage& img){
            images.push_back(img.NLagrangeInterpolation(stoi(options["n"])));
        }}
    };

    // Call functions
    if (functionMap.find(options["method"]) != functionMap.end()) {
        functionMap[options["method"]](augmentedImage);
    }else{
        string error = "Method '"+options["method"]+"' not found.";
        die(error.c_str());
    }

    if(options["display"] == "true"){        
        display(images);
    }
    if(options["save"] == "true")
        save(options["savePath"], images);

    return 0;
}
