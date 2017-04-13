#include "Compass.h"

Compass::Compass(I2C * channelIn, SSD1351_SPI * displayIn, unsigned int foregroundColorIn, unsigned int backgroundColorIn, int xOffsetIn, int yOffsetIn) {
    channel = channelIn;

    // minimum data output rate
    write_reg(ADDRESS_MAGNETISM,CRA_REG_M,0x18);

    // gain setting
    write_reg(ADDRESS_MAGNETISM,CRB_REG_M,0x20);

    // continuous-conversion mode
    write_reg(ADDRESS_MAGNETISM,MR_REG_M,0);

    foregroundColor = foregroundColorIn;
    backgroundColor = backgroundColorIn;
    xOffset = xOffsetIn;
    yOffset = yOffsetIn;
    display = displayIn;
    mx = 0;
    my = 0;
    old_direction = "";

    printf("compass init\r\n");

}

bool Compass::write_reg(int addr_i2c,int addr_reg, char v) { //What are these addresses????????????????
        char data[2] = {addr_reg, v};
        return channel->write(addr_i2c, data, 2) == 0;
}

bool Compass::receive(char address, char outAddress, char *buffer) {
        if (READ_LENGTH > 1) outAddress |= 0x80;
        return channel->write(address, &outAddress, 1, true) == 0 && channel->read(address, buffer, READ_LENGTH) == 0;
}

void Compass::draw() {
    bool ready = readMagnetismVector();
    //if (!ready)
    float heading = calc_heading(mx,my);
    string dir = direction(heading);
    if (dir != old_direction) {
        display->printF(backgroundColor, xOffset, yOffset, old_direction.c_str());
        display->printF(foregroundColor, xOffset, yOffset, dir.c_str());
    }
    old_direction = dir;
    // printf("Heading : %f\tDirection : %s\r\n", heading, dir);
}

bool Compass::readMagnetismVector() {
    char mag[READ_LENGTH];
    if (receive(ADDRESS_MAGNETISM, OUT_X_MAG, mag)) {                   //can we just use data ready instead????????????
        mx = float(short(mag[0] << EIGHT_BIT_SHIFT | mag[1])) / GAIN_X;
        my = float(short(mag[4] << EIGHT_BIT_SHIFT | mag[5])) / GAIN_Y;
        //mz = float(short(mag[2] << EIGHT_BIT_SHIFT | mag[3])) / GAIN_Z;
        return true;
    }
    return false;
}

float Compass::calc_heading(float mx, float my) {
    // Calculate the angle of the vector y,x
    float heading = (atan2(my,mx) * 180) / Pi;

    // Normalize to 0-360
    if (heading < 0) {
        heading = 360 + heading;
    }
    currHeading = heading;
    return heading;
}

std::string Compass::direction(float heading){
    std::string dir;
    int trueDir = heading/22.5;
    std::string directions[] = {"N", "NE", "NE", "E", "E", "SE", "SE", "S", "S", "SW", "SW", "W", "W", "NW", "NW", "N"};
    return directions[trueDir];
}

void Compass::callback() {
    if (global_counter % 100 == 0) {
        draw();
    }
}

float Compass::getHeading(){
    return currHeading;
}
