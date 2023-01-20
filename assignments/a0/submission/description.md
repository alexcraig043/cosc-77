#### Cosc 77 Assignment 0

# Sailboat

### By: Alex Craig

<iframe src="https://giphy.com/embed/Pme8kYsIL24YRjr9NY" width="480" height="354" ></iframe>

#### Description

Hi I'm Alex, a '25 from Mattapoisett, Massachusetts. For my project I made a sailboat because I live near the water and enjoy spending time sailing during the summer. I also love watching sunrises and sunsets over the ocean, so I chose to add those into my canvas too.

For technical implementation, I created an `inQuad` function to check if a pixel is inside a quadrilateral (using the provided `inTriangle` function) in order to draw my boat hull and mast. For the sail, I used the `inTriangle` function. I then created a `belowSine` function to check if a pixel is below a sine wave which I used to draw the waves. To make the waves move, I offset the horizontal position of the sine wave by an iTime multiplier. I then added a sun and moon using the provided `inCircle` function and positioned them 180 degrees apart using the provided `polar2cart` function. To make the sun and moon rotate around the screen, I multiply their polar coordinate angle by an iTime multiplier. The sky color is dependant on the sun's y position, which allows me to change the color in order to draw the sunset and night sky depending on how low the sun is on the screen.
