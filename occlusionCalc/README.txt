OCCLUSIONCALC

## General Information

-   **Author Information:**
    -   Principal Coder: Atiyah B. Ellerbee
-   **Keywords:** [Relevant keywords for discoverability]

## Data/File Overview

-   **File List and Descriptions:**
    -   occluCalc.cpp: [Code for occluCalc.exe, the overall algroithm to see occlusion output. ]
    -   occluCalc.exe: [Exe debugged from occluCalc.cpp]
    -   plotRecs.py: [parses rects.txt and outputs the information into matplot]
    -   rects.csv : [CSV for C++ exe to output random rectangle generated into for matplotlib output]


-   **File Formats:** cpp, exe, py, txt, csv
-   **File Relationships/Structure:** occluCalc.cpp informs occluCalc.exe, which overall populates rects.txt 
        with each new rectangle generation. plotRecs.py then runs matplotlib
-   **Date of Last Update/Version:** 9/4/2025
-   **Related Data:** some help from textbook and debugging help from chatGPT

## Usage/Access Information

-   **Description:** A set of programs to randomly generate rectangles on a plane, with a center point, then
            calculate the overall occlusion of the visible rectangles from the central point. the rectangles should
            always be random and no two generations should be the same.
-   **Prerequisites:** python complier, matplotlib package, cpp complier, cpp extension
-   **Installation/Setup:** dependent on your system, see screen recording for execution expected on a windows machine
-   **Instructions for Use:** 
            debug and run occluCalc, input the number for the random generated rectangles,
            the rectangles dimensions and coordinates will be populated into rects.csv automatically. 
            run plotRects.py to see the plots / outputs of the origin, occlusion, and rectangles on
            quadrents in matplot.
-   **Known Issues/Limitations:** over 10 rectangles overlap with the central point

## Contact Information

-   **For Questions/Support:** aellerbe@purdue.edu