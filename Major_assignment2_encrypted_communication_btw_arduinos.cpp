/*
    Encrypted Arduino Communication Part 1 Solution
    CMPUT 274 Fall 2019
    Written by Ian DeHaan
    Tweaks by Zac Friggstad
*/
#include <Arduino.h>

// Constants from the assn. spec
// Primes: 307, 311
const uint32_t serverPublicKey = 7;
const uint32_t serverPrivateKey = 27103;
const uint32_t serverModulus = 95477;

// Generator: 271, 313
const uint32_t clientPublicKey = 11;
const uint32_t clientPrivateKey = 38291;
const uint32_t clientModulus = 84823;

const int serverPin = 13;

/*
    Returns true if arduino is server, false if arduino is client
*/
bool isServer() {
    if (digitalRead(serverPin) == HIGH) {
        return true;
    } else {
        return false;
    }
}

/*
    Compute and return (a*b)%m
    Note: m must be less than 2^31
    Arguments:
        a (uint32_t): The first multiplicant
        b (uint32_t): The second multiplicant
        m (uint32_t): The mod value
    Returns:
        result (uint32_t): (a*b)%m
*/
uint32_t multMod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t result = 0;
    uint32_t dblVal = a%m;
    uint32_t newB = b;

    // This is the result of working through the worksheet.
    // Notice the extreme similarity with powmod.
    while (newB > 0) {
        if (newB & 1) {
            result = (result + dblVal) % m;
        }
        dblVal = (dblVal << 1) % m;
        newB = (newB >> 1);
    }

    return result;
}


/*
    NOTE: This was modified using our multMod function, but is otherwise the
    function powModFast provided in the lectures.

    Compute and return (a to the power of b) mod m.
	  Example: powMod(2, 5, 13) should return 6.
*/
uint32_t powMod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t result = 1 % m;
    uint32_t sqrVal = a % m;  // stores a^{2^i} values, initially 2^{2^0}
    uint32_t newB = b;

    // See the lecture notes for a description of why this works.
    while (newB > 0) {
        if (newB & 1) {  // evalutates to true iff i'th bit of b is 1 in the i'th iteration
            result = multMod(result, sqrVal, m);
        }
        sqrVal = multMod(sqrVal, sqrVal, m);
        newB = (newB >> 1);
    }

    return result;
}



/** Writes an uint32_t to Serial3, starting from the least-significant
 * and finishing with the most significant byte.
 */
void uint32_to_serial3(uint32_t num) {
    Serial3.write((char) (num >> 0));
    Serial3.write((char) (num >> 8));
    Serial3.write((char) (num >> 16));
    Serial3.write((char) (num >> 24));
}


/** Reads an uint32_t from Serial3, starting from the least-significant
 * and finishing with the most significant byte.
 */
uint32_t uint32_from_serial3() {
    uint32_t num = 0;
    num = num | ((uint32_t) Serial3.read()) << 0;
    num = num | ((uint32_t) Serial3.read()) << 8;
    num = num | ((uint32_t) Serial3.read()) << 16;
    num = num | ((uint32_t) Serial3.read()) << 24;
    return num;
}


/*
    Encrypts using RSA encryption.

    Arguments:
        c (char): The character to be encrypted
        e (uint32_t): The partner's public key
        m (uint32_t): The partner's modulus

    Return:
        The encrypted character (uint32_t)
*/
uint32_t encrypt(char c, uint32_t e, uint32_t m) {
    return powMod(c, e, m);
}


/*
    Decrypts using RSA encryption.

    Arguments:
        x (uint32_t): The communicated integer
        d (uint32_t): The Arduino's private key
        n (uint32_t): The Arduino's modulus

    Returns:
        The decrypted character (char)
*/
char decrypt(uint32_t x, uint32_t d, uint32_t n) {
    return (char) powMod(x, d, n);
}

/*
    Core communication loop
    d, n, e, and m are according to the assignment spec
*/
void communication(uint32_t d, uint32_t n, uint32_t e, uint32_t m) {
    // Consume all early content from Serial3 to prevent garbage communication
    while (Serial3.available()) {
        Serial3.read();
    }

    // Enter the communication loop
    while (true) {
        // Check if the other Arduino sent an encrypted message.
        if (Serial3.available() >= 4) {
            // Read in the next character, decrypt it, and display it
            uint32_t read = uint32_from_serial3();
            Serial.print(decrypt(read, d, n));
        }

        // Check if the user entered a character.
        if (Serial.available() >= 1) {
            char byteRead = Serial.read();
            // Read the character that was typed, echo it to the serial monitor,
            // and then encrypt and transmit it.
            if ((int) byteRead == '\r') {
                // If the user pressed enter, we send both '\r' and '\n'
                Serial.print('\r');
                uint32_to_serial3(encrypt('\r', e, m));
                Serial.print('\n');
                uint32_to_serial3(encrypt('\n', e, m));
            } else {
                Serial.print(byteRead);
                uint32_to_serial3(encrypt(byteRead, e, m));
            }
        }
    }
}


/*
    Performs basic Arduino setup tasks.
*/
void setup() {
    init();
    Serial.begin(9600);
    Serial3.begin(9600);

    Serial.println("Welcome to Arduino Chat!");
}


/*
    The entry point to our program.
*/
int main() {
    setup();
    uint32_t d, n, e, m;

    // Determine our role and the encryption keys.
    if (isServer()) {
        Serial.println("Server");
        d = serverPrivateKey;
        n = serverModulus;
        e = clientPublicKey;
        m = clientModulus;
    } else {
        Serial.println("Client");
        d = clientPrivateKey;
        n = clientModulus;
        e = serverPublicKey;
        m = serverModulus;
    }

    // Now enter the communication phase.
    communication(d, n, e, m);

    // Should never get this far (communication has an infite loop).

    return 0;
}
