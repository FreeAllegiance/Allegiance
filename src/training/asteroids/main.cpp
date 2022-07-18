#include <iostream>
#include "ZAssert.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

using namespace std;

float   UnitRandom (void)
{
    return rand () / static_cast<float> (RAND_MAX);
}

int	main (int argc, char** argv)
{
    srand (static_cast<unsigned int> (time (0)));
	for (int i = 0; i < argc; i++)
        if (argv[i][0] == '-')
            if (strcmp (argv[i], "-count") == 0)
                if (argc > (i + 1))
                {
                    int iCount = atoi (argv[i + 1]);
                    // aspect ratio 5x5x1
                    float   fWidth = sqrtf (static_cast<float> (iCount));
                    int     iWidth = static_cast<int> (floorf (fWidth));
                    int     iHeight = iCount / iWidth;
                    //cout << "Count: " << iCount << endl << "Width: " << iWidth << endl << "Height: " << iHeight << endl;
                    // world size 10,000x10,000x2,000
                    float   fWidthMultiplier = 10000.0f / static_cast<float> (iWidth);
                    float   fHeightMultiplier = 10000.0f / static_cast<float> (iHeight);
                    iCount = 0;
                    for (int j = 0; j < iHeight; j++)
                    {
                        for (int k = 0; k < iWidth; k++)
                        {
                            float   radius = (UnitRandom () * 200.0f) + 150.0f;
                            float   mass = radius * radius;
                            float   hitpoints = radius * 5.0f;
                            float   signature = radius / 10.0f;
                            float   rotation = 0.1 + (UnitRandom () * 0.15f);
                            bool    isHelium = (UnitRandom () < 0.1f) ? true : false;
                            float   ore = 1000.0f + (UnitRandom () * 10000.0f);
                            float   x = ((UnitRandom () + static_cast<float> (k)) * fWidthMultiplier) - 5000.0f;
                            float   y = ((UnitRandom () + static_cast<float> (j)) * fWidthMultiplier) - 5000.0f;
                            float   z = (UnitRandom () * 2000.0f) - 1000.0f;
                            cout << "1024\t1\t" << iCount++ << "\t" << ore << "\t" << (isHelium ? 1 : 0) << "\t" << static_cast<int> (hitpoints) << "\t" << static_cast<int> (radius) << "\t" << (isHelium ? "bgrnd56" : "bgrnd03") << "\t\tmeteoricon\t" << mass << "\t" << signature << "\t" << x << "\t" << y << "\t" << z << "									\t" << rotation << "\tName" << endl;
                        }
                    }
                    return 0;
                }
    cout << "gimme a count (-count 10)" << endl;
	return 1;
}
