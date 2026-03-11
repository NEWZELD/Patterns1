#include <iostream>
#include <string>

using namespace std;
// Íîâûé èíòåðôåéñ ïðèíòåðà (USB)
class USBPrinter {
public:
    virtual string printUSB(const string& data, int quality) = 0;
    virtual ~USBPrinter() {}
};

// Íîâûé èíòåðôåéñ ñêàíåðà (USB)
class USBScanner {
public:
    virtual string scanUSB(int quality) = 0;
    virtual ~USBScanner() {}
};


// Ñòàðûé ïðèíòåð (LPT)
class OldLPTPrinter {
public:
    string printLPT(const std::string& data) {
        return "Ñòàðûé ïðèíòåð ïå÷àòàåò ïî LPT: " + data;
    }
};

// Ñòàðûé ñêàíåð (LPT)
class OldLPTScanner {
public:
    string scanLPT() {
        return "Ñêàíèðîâàíèå ÷åðåç ñòàðûé LPT-ñêàíåð";
    }
};


// Àäàïòåð ïðèíòåðà
class LPTToUSBPrinterAdapter : public USBPrinter {
private:
    OldLPTPrinter* oldPrinter;

public:
    LPTToUSBPrinterAdapter(OldLPTPrinter* p) : oldPrinter(p) {}

    string printUSB(const string& data, int quality) override {
        string adapted = data + " | êà÷åñòâî: " + to_string(quality);
        return oldPrinter->printLPT(adapted + " (àäàïòèðîâàíî äëÿ USB)");
    }
};

// Àäàïòåð ñêàíåðà
class LPTToUSBScannerAdapter : public USBScanner {
private:
    OldLPTScanner* oldScanner;

public:
    LPTToUSBScannerAdapter(OldLPTScanner* s) : oldScanner(s) {}

    string scanUSB(int quality) override {
        string result = oldScanner->scanLPT();
        return result + " | êà÷åñòâî: " + std::to_string(quality) + " USB àäàïòåð";
    }
};



class ClassPrinterAdapter : public USBPrinter, public OldLPTPrinter {
public:
    string printUSB(const std::string& data, int quality) override {
        string adapted = data + " | êà÷åñòâî: " + to_string(quality);
        return printLPT(adapted + " Êëàññ àäàïòåð");
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    OldLPTPrinter oldPrinter;
    OldLPTScanner oldScanner;

    USBPrinter* printAdapter = new LPTToUSBPrinterAdapter(&oldPrinter);
    USBScanner* scannAdapter = new LPTToUSBScannerAdapter(&oldScanner);

    cout << printAdapter->printUSB("Äîêóìåíò", 90) << endl;
    cout << scannAdapter->scanUSB(75) << endl;

    USBPrinter* classAdapter = new ClassPrinterAdapter();
    cout << classAdapter->printUSB("Ôîòî", 100) << endl;

    delete printAdapter;
    delete scannAdapter;
    delete classAdapter;


    return 0;
}
