class ACEMessenger {
   public:
    ACEMessenger();
    ~ACEMessenger();

    void AddToPhotonCount();
    void resetCounts();
    int PhotonCount();

    void AddHits(int);
    void AddHits(double x, double y, int);
    double GetAverageNumHits();
    int *getSpectrum();
    int getNumEvents();
    double **getSpaceSpectrum();

   private:
    int *spectrum;
    int photonCount;
    int numHits;
    int numEvents;
    int spaceSpecSize;
    double **spaceSpectrum;
};

extern ACEMessenger *messenger;
