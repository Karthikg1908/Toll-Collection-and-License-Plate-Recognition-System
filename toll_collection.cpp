#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <tesseract/capi.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#include <leptonica/allheaders.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string.h>

using namespace cv;

// Global variables for OpenGL window
int windowWidth = 600;
int windowHeight = 600;
bool spacePressed = false; // Flag to track if space button is pressed
bool showInstructions = false;
bool showPrices = false;
bool countdownStarted = false;
int countdownTime = 5; // Countdown time in seconds
int frameCount = 0;

// Vehicle parameters
float vehicleSpeed = 2.0;

// Toll parameters
float tollBoothPosition = 400.0;
float motorcycleTollAmount = 25.0;
float carTollAmount = 50.0;
float busTollAmount = 80.0;
float truckTollAmount = 110.0;
float vanTollAmount = 70.0;
float bicycleTollAmount = 10.0;

float collectedTolls = 0.0;

// Enum for vehicle types
typedef enum {
    BICYCLE,
    MOTORCYCLE,
    CAR,
    VAN,
    BUS,
    TRUCK,
    SUV,
    TAXI,
    AMBULANCE,
} VehicleType;

// Structure for vehicles
typedef struct {
    float position;
    VehicleType type;
    bool passedToll; // Flag to indicate if the vehicle has passed the toll
} Vehicle;

// Array of vehicles
Vehicle vehicles[50]; // Increased the size to accommodate more vehicles
int numVehicles = 0;

// Project details
const char* projectName = "Toll Collection & License Plate Recognition System";
 const char* studentName = "Karthik G";
const char* guideName = "Prof. Usha M";
const char* teammate1 = "   Karthik G";
const char* teammate2 = "   Abhishek B C";
const char* teammate3 = "   Prashanth";
const char* teammate4 = "   Lakshminarsimha P K";

// Function declarations
Mat preprocessImage(Mat img);
std::vector<Rect> detectNumberPlates(Mat edged);
std::string recognizeNumberPlate(Mat img, Rect plateRect);
void drawVehicle(float xPos, float yPos, VehicleType type);
void drawTollBooth(float xPos);
void drawText(float x, float y, const char* text);
void updateVehicles();
void drawInstructions();
void drawPrices();
void processImage(); // Function to process image when Enter key is pressed
void startCountdown(); // Function to start the countdown

// Function to preprocess the image
Mat preprocessImage(Mat img) {
    Mat gray, blurred, edged;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blurred, Size(5, 5), 0);
    Canny(blurred, edged, 50, 200, 3);
    return edged;
}

// Function to detect potential number plates
std::vector<Rect> detectNumberPlates(Mat edged) {
    std::vector<std::vector<Point>> contours;
    std::vector<Rect> plates;

    findContours(edged, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        if (rect.width > rect.height && rect.area() > 1000) {
            plates.push_back(rect);
        }
    }
    return plates;
}

// Function to perform OCR on detected number plates
std::string recognizeNumberPlate(cv::Mat img, cv::Rect plateRect) {
    // Extract the region of interest (ROI) from the input image
    cv::Mat croppedImage = img(plateRect);

    // Convert the cropped image to a Pix structure
    std::vector<uchar> buf;
    cv::imencode(".png", croppedImage, buf);
    Pix *pixImage = pixReadMem(buf.data(), buf.size());

    if (!pixImage) {
        printf("Error: Could not convert Mat to Pix.\n");
        return "";
    }

    // Initialize Tesseract API
    TessBaseAPI *api = TessBaseAPICreate();
    if (TessBaseAPIInit3(api, NULL, "eng")) {
        printf("Error: Could not initialize Tesseract.\n");
        TessBaseAPIDelete(api);
        pixDestroy(&pixImage);
        return "";
    }

    // Set image
    TessBaseAPISetImage2(api, pixImage);

    // Perform OCR
    TessBaseAPIRecognize(api, NULL);
    char *text = TessBaseAPIGetUTF8Text(api);
    std::string numberPlate(text);

    // Clean up
    TessDeleteText(text);
    TessBaseAPIDelete(api);
    pixDestroy(&pixImage);

    return numberPlate;
}

// Function to draw a vehicle
void drawVehicle(float xPos, float yPos, VehicleType type) {
    switch (type) {
        case BICYCLE:
            // Draw line for bicycle
            glColor3f(0.1, 0.7, 0.3); // Green color
            glBegin(GL_LINES);
            glVertex2f(xPos - 10, yPos);
            glVertex2f(xPos + 10, yPos);
            glVertex2f(xPos + 10, yPos);
            glVertex2f(xPos + 20, yPos + 10);
            glVertex2f(xPos + 10, yPos);
            glVertex2f(xPos + 20, yPos - 10);
            glEnd();
            break;
        case MOTORCYCLE:
            // Draw circle for motorcycle
            glColor3f(0.9, 0.1, 0.1); // Red color
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; i++) {
                float theta = i * M_PI / 180;
                glVertex2f(xPos + 10 * cos(theta), yPos + 10 * sin(theta));
            }
            glEnd();
            break;
        case CAR:
            // Draw square for car
            glColor3f(0.5, 0.5, 0.5); // Gray color
            glBegin(GL_POLYGON);
            glVertex2f(xPos - 10, yPos - 10);
            glVertex2f(xPos + 10, yPos - 10);
            glVertex2f(xPos + 10, yPos + 10);
            glVertex2f(xPos - 10, yPos + 10);
            glEnd();
            break;
        case VAN:
            // Draw hexagon for van
            glColor3f(0.7, 0.5, 0.3); // Brown color
            glBegin(GL_POLYGON);
            glVertex2f(xPos - 10, yPos - 5);
            glVertex2f(xPos + 10, yPos - 5);
            glVertex2f(xPos + 15, yPos);
            glVertex2f(xPos + 10, yPos + 5);
            glVertex2f(xPos - 10, yPos + 5);
            glVertex2f(xPos - 15, yPos);
            glEnd();
            break;
        case BUS:
            // Draw triangle for bus
            glColor3f(0.8, 0.6, 0.2); // Yellowish color
            glBegin(GL_POLYGON);
            glVertex2f(xPos, yPos - 10);
            glVertex2f(xPos + 20, yPos + 10);
            glVertex2f(xPos - 20, yPos + 10);
            glEnd();
            break;
        case TRUCK:
            // Draw pentagon for truck
            glColor3f(0.3, 0.3, 0.7); // Blue color
            glBegin(GL_POLYGON);
            glVertex2f(xPos - 10, yPos - 5);
            glVertex2f(xPos + 10, yPos - 5);
            glVertex2f(xPos + 15, yPos);
            glVertex2f(xPos, yPos + 10);
            glVertex2f(xPos - 15, yPos);
            glEnd();
            break;
        case SUV:
            // Draw trapezoid for SUV
            glColor3f(0.4, 0.2, 0.1); // Dark brown color
            glBegin(GL_POLYGON);
            glVertex2f(xPos - 15, yPos - 5);
            glVertex2f(xPos + 15, yPos - 5);
            glVertex2f(xPos + 20, yPos + 5);
            glVertex2f(xPos - 20, yPos + 5);
            glEnd();
            break;
        case TAXI:
            // Draw rectangle for taxi
            glColor3f(1.0, 0.8, 0.0); // Yellow color
            glBegin(GL_POLYGON);
            glVertex2f(xPos - 10, yPos - 5);
            glVertex2f(xPos + 10, yPos - 5);
            glVertex2f(xPos + 10, yPos + 5);
            glVertex2f(xPos - 10, yPos + 5);
            glEnd();
            break;
        case AMBULANCE:
            // Draw cross for ambulance
            glColor3f(1.0, 0.0, 0.0); // Red color
            glBegin(GL_LINES);
            glVertex2f(xPos - 10, yPos - 5);
            glVertex2f(xPos + 10, yPos + 5);
            glVertex2f(xPos - 10, yPos + 5);
            glVertex2f(xPos + 10, yPos - 5);
            glEnd();
            break;
    }
}

// Function to draw the toll booth
void drawTollBooth(float xPos) {
    glColor3f(0.3, 0.7, 0.3); // Green color
    glBegin(GL_POLYGON);
    glVertex2f(xPos, 0);
    glVertex2f(xPos + 100, 0);
    glVertex2f(xPos + 100, windowHeight);
    glVertex2f(xPos, windowHeight);
    glEnd();
}

// Function to display text
void drawText(float x, float y, const char* text) {
    glColor3f(0.0, 0.0, 0.0); // Black color
    glRasterPos2f(x, y);

    // Enable UTF-8 character encoding
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

// Function to update vehicle positions
void updateVehicles() {
    for (int i = 0; i < numVehicles; ++i) {
        if (!vehicles[i].passedToll) {
            vehicles[i].position += vehicleSpeed;
            if (vehicles[i].position >= tollBoothPosition) {
                switch (vehicles[i].type) {
                    case BICYCLE:
                        collectedTolls += bicycleTollAmount;
                        break;
                    case MOTORCYCLE:
                        collectedTolls += motorcycleTollAmount;
                        break;
                    case CAR:
                        collectedTolls += carTollAmount;
                        break;
                    case VAN:
                        collectedTolls += vanTollAmount;
                        break;
                    case BUS:
                        collectedTolls += busTollAmount;
                        break;
                    case TRUCK:
                        collectedTolls += truckTollAmount;
                        break;
                    case SUV:
                        collectedTolls += carTollAmount * 1.5; // SUV toll is 1.5 times car toll
                        break;
                    case TAXI:
                        collectedTolls += carTollAmount; // Taxi toll same as car toll
                        break;
                    case AMBULANCE:
                        // Ambulance toll exempted
                        break;
                    default:
                        break;
                }
                vehicles[i].passedToll = true;
            }
        }
    }

    // Remove vehicles that have passed the toll
    int newNumVehicles = 0;
    for (int i = 0; i < numVehicles; ++i) {
        if (!vehicles[i].passedToll) {
            vehicles[newNumVehicles++] = vehicles[i];
        }
    }
    numVehicles = newNumVehicles;
}

void drawInstructions() {
    // Display toll collection instructions on the screen
    glColor3f(0.2, 0.2, 0.8); // Navy blue color
    drawText(10, windowHeight - 30, "Toll Collection and License Plate Recognition System Instructions");

    // Display instructions for adding vehicles
    glColor3f(0.0, 0.0, 0.0); // Black color
    drawText(10, windowHeight - 60, "Press 1-9 to add vehicles like bicycle, motorcycle, car, van, bus, truck, SUV, taxi, or ambulance.");
    drawText(10, windowHeight - 90, "Press 'Space Bar': Start/Stop");
    drawText(10, windowHeight - 110, "Hit 'Enter' to begin number plate detection.");
    drawText(10, windowHeight - 140, "Press 'P' for price details");
}

// Function to draw vehicle toll prices
void drawPrices() {
    // Display vehicle toll prices
    glColor3f(0.0, 0.0, 0.0); // Black color
    drawText(10, windowHeight - 200, "Vehicle Toll Prices:");
    drawText(10, windowHeight - 220, "Bicycle: ₹10.00");
    drawText(10, windowHeight - 240, "Motorcycle: ₹25.00");
    drawText(10, windowHeight - 260, "Car: ₹50.00");
    drawText(10, windowHeight - 280, "Van: ₹70.00");
    drawText(10, windowHeight - 300, "Bus: ₹80.00");
    drawText(10, windowHeight - 320, "Truck: ₹110.00");
    drawText(10, windowHeight - 340, "SUV: ₹75.00");
    drawText(10, windowHeight - 360, "Taxi: ₹50.00");
    drawText(10, windowHeight - 380, "Ambulance: Toll Exempted");
}

// Function to process image
void processImage() {
    // Load the image
    Mat image = imread("/home/karthi/Documents/n3.jpeg");
    if (image.empty()) {
        printf("Error: Could not read the image.\n");
        return;
    }

    // Preprocess the image
    Mat processedImage = preprocessImage(image);

    // Detect number plates
    std::vector<Rect> plates = detectNumberPlates(processedImage);

    // Recognize number plates and print them
    for (size_t i = 0; i < plates.size(); i++) {
        std::string plateText = recognizeNumberPlate(image, plates[i]);
        printf("Detected Number Plate: %s\n", plateText.c_str());
    }
}

// Function to start the countdown
void startCountdown() {
    countdownStarted = true;
}

// Function to display teammate names
void displayTeammates() {
    drawText(10, windowHeight - 80, "Project Developed By:");
    drawText(10, windowHeight - 100, teammate1);
    drawText(10, windowHeight - 120, teammate2);
    drawText(10, windowHeight - 140, teammate3);
    drawText(10, windowHeight - 160, teammate4);
}


// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!countdownStarted) {
        // Display project details and countdown
        drawText(10, windowHeight - 20, projectName);
        drawText(windowWidth / 2 - 100, windowHeight / 2, "Designed by : ");
        drawText(windowWidth / 2 + 50, windowHeight / 2, studentName);
        drawText(windowWidth / 2 - 100, windowHeight / 2 - 20, "Under the guidance of : ");
        drawText(windowWidth / 2 + 50, windowHeight / 2 - 20, guideName);
        char countdownText[50];
        sprintf(countdownText, "Starting in %d seconds...", countdownTime);
        drawText(windowWidth / 2 - 70, windowHeight / 2 - 50, countdownText);
        displayTeammates(); // Call to display teammates
    } else {
        if (showInstructions) {
            drawInstructions(); // Display instructions if needed
        } else if (showPrices) {
            drawPrices(); // Display vehicle toll prices if needed
        } else {
            drawTollBooth(tollBoothPosition); // Draw toll booth
            // Draw vehicles
            for (int i = 0; i < numVehicles; ++i) {
                drawVehicle(vehicles[i].position, 100 + i * 40, vehicles[i].type);
            }
            // Display collected toll amount
            char tollText[100];
            sprintf(tollText, "Collected Tolls: ₹%.2f", collectedTolls);
            drawText(10, windowHeight - 20, tollText);
        }
    }

    // Display additional messages at the bottom left corner
    drawText(10, 20, "Press 'i' for instructions");
    drawText(10, 40, "Press 'q' to exit");

    glutSwapBuffers();
}



// Keyboard callback function
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Existing key cases...
        case 27: // ESC key
            exit(0);
            break;
        case 'i':
        case 'I':
            showInstructions = !showInstructions; // Toggle instructions display
            break;
        case 'p':
        case 'P':
            showPrices = !showPrices; // Toggle vehicle toll prices display
            break;
        case '1':
            vehicles[numVehicles++] = (Vehicle){0, BICYCLE, false};
            break;
        case '2':
            vehicles[numVehicles++] = (Vehicle){0, MOTORCYCLE, false};
            break;
        case '3':
            vehicles[numVehicles++] = (Vehicle){0, CAR, false};
            break;
        case '4':
            vehicles[numVehicles++] = (Vehicle){0, VAN, false};
            break;
        case '5':
            vehicles[numVehicles++] = (Vehicle){0, BUS, false};
            break;
        case '6':
            vehicles[numVehicles++] = (Vehicle){0, TRUCK, false};
            break;
        case ' ':
            spacePressed = !spacePressed; // Toggle space button state
            break;
        case '7':
            vehicles[numVehicles++] = (Vehicle){0, SUV, false};
            break;
        case '8':
            vehicles[numVehicles++] = (Vehicle){0, TAXI, false};
            break;
        case '9':
            vehicles[numVehicles++] = (Vehicle){0, AMBULANCE, false};
            break;
        case '\r': // Enter key
            processImage();
            break;
        case 'q': // Stop toll collection and display total amount collected
            printf("Total amount collected: ₹%.2f\n", collectedTolls);
            exit(0);
            break;
    }
}
// Timer callback function
void timer(int value) {
    if (!countdownStarted) {
        // Countdown before starting simulation
        if (frameCount % 60 == 0 && countdownTime > 0) {
            countdownTime--;
        }
        if (countdownTime == 0) {
            startCountdown();
        }
    } else {
        if (spacePressed) {
            updateVehicles(); // Update vehicle positions only when space is pressed
        }
    }
    frameCount++;
    glutPostRedisplay();
    glutTimerFunc(1500 / 60, timer, 0); // 60 fps
}

// Reshape callback function
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    windowWidth = w;
    windowHeight = h;
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);

    // Create main window
    glutCreateWindow("Toll Collection and License Plate Recognition System");
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}