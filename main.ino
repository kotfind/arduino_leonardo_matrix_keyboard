#include <Keyboard.h>

const int COLUMNS = 12;  // connected to digital ports
const int COLUMNS_OFFSET = 2;
const int ROWS = 4;       // connected to analog ports

bool state[ROWS][COLUMNS];     // 0 -- not pressed; 1 -- pressed
char keyLayout[ROWS][COLUMNS] = {
{0xB3,  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p', 0xB2},
{0xB1,  'a',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '"'},
{0x81,  'z',  'x',  'c',  'v',  'b',  'n',  'm',  ',',  '.',  '/', 0xB0},
{0x80, 0x82, 0x83, -0x0, -0x0,  ' ',  ' ', -0x0, 0xD8, 0xD9, 0xDA, 0xD7},
};

void setup() {
    Serial.begin(9600);
    Keyboard.begin();

    for (int r = 0; r < ROWS; ++r) {
        pinMode(A0 + r, INPUT);
        digitalWrite(A0 + r, HIGH);
    }
    for (int c = 0; c < COLUMNS; ++c) {
        pinMode(c, OUTPUT);
    }

    for (int c = 0; c < COLUMNS; ++c)
        for (int r = 0; r < ROWS; ++r)
            state[r][c] = 0;
}

void processKey(int r, int c, bool state) {
    if (r == 3 && c == 3)
        return;
    if (r == 3 && c == 4)
        return;
    if (r == 3 && c == 9)
        return;

    if (state == 1)
        Keyboard.press(keyLayout[r][c]);
    else
        Keyboard.release(keyLayout[r][c]);
}

void loop() {
    for (int c = 0; c < COLUMNS; ++c) {
        digitalWrite(c + COLUMNS_OFFSET, LOW);
        for (int r = 0; r < ROWS; ++r) {
            bool s = (digitalRead(A0 + r) == LOW);
            if (s != state[r][c]) {
                state[r][c] = s;
                Serial.println("R" + String(r) + "\t" "C" + String(c) + "\t" + (s == 1 ? "Pressed" : "Released"));

                processKey(r, c, s);
            }
        }
        digitalWrite(c + COLUMNS_OFFSET, HIGH);
    }
    delay(100);
}
