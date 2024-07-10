---

# Toll Collection System

This project demonstrates a toll collection system using OpenCV for image processing, Tesseract for OCR, and FreeGLUT for OpenGL rendering.

## Step 1: Install Necessary Dependencies

Make sure you have the required libraries installed. Open a terminal and run the following commands:

```bash
sudo apt install build-essential cmake pkg-config
sudo apt install libopencv-dev
sudo apt install tesseract-ocr libtesseract-dev libleptonica-dev
sudo apt install freeglut3 freeglut3-dev
sudo apt install libglu1-mesa libglu1-mesa-dev mesa-common-dev
sudo apt install tesseract-ocr-eng
```

## Step 2: Copy the Code

Save the following code as `toll_collection.cpp`:

```cpp
// Add toll_collection.cpp C++ code here
```

## Step 3: Compile the Program

Run the following command to compile the program:

```bash
g++ -o toll_collection toll_collection.cpp -lGL -lGLU -lglut `pkg-config --cflags --libs opencv4` -ltesseract -llept
```

## Step 4: Run the Program

Execute the compiled program with:

```bash
./toll_collection
```

## Step 5: Interact with the Program

Use the keyboard as described in the code comments:

- Press `1-9` to add different types of vehicles.
- Press `Space Bar` to start/stop the vehicle movement.
- Press `Enter` to process an image for license plate detection.
- Press `i` for instructions.
- Press `p` for toll prices.
- Press `q` to exit and show the total collected toll.

## Troubleshooting

- **Missing Image**: Ensure the image path provided in the code exists. If not, change the path to an existing image.
- **Compilation Errors**: If you encounter any errors during compilation, ensure all dependencies are correctly installed, and paths are correctly set.

---

## Check The Result

Starting Window

![image](https://github.com/Karthikg1908/Toll-Collection-and-License-Plate-Recognition-System/assets/86306862/bacce3bf-e68e-4ef6-88e1-503b35c2e2c5)

Home Page

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/ea1cca46-ac7e-4dbd-9e1d-248a4fc38170)

Instructions Window

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/d12f6e42-f035-43a4-b53f-f15ff97ed5bc)

Vehicle Toll Prices

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/aa9c3b0e-9c00-4ad0-8266-6dc0714503bc)

Vehicles Starting Point

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/45855f2f-68fd-4193-b519-c4119b3ccc91)

Vehicle Movement 

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/4b52e720-5a1e-46f3-b195-89cdf01f6012)

Total Amount Collected

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/597f35e1-29c6-4b85-a9a4-76035eab2734)

Sample Number Plate 1

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/c2dc842b-57b4-4d45-9ace-b828780b33ab)

Number Plate Recognition

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/7c5f4c45-da0a-45ef-b7e6-ce597996ec75)

Sample Number Plate 2

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/114ceb54-9c4b-4023-ab20-2bae90f4a234)

Number Plate Recognition

![image](https://github.com/Karthikg1908/Toll-Collection-License-Plate-Recognition-System/assets/86306862/7d67964e-ab5f-4071-8705-a2712f77b847)
