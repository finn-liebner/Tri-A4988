////////////////////////////////////////////////////////////////
// Bibliothek zur Ansteuerung von drei A4988-Treibern         //
// Kompatibel zur Arduino-Plattform                           //
// Stand 30/01/2020 - Entwickelt von Finn Liebner             //
////////////////////////////////////////////////////////////////

class KL
{
  public:
    void Init(void);
    void Interval(unsigned int v);
    void Microsteps(byte res);
    void Rotate(int x, int y, int z);
};

static KL A4988;

////////////////////////////////////////////////////////////////
