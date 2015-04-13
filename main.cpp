#include <iostream>
#include "Eigen/Eigen"
#include "FFTW/fftw3.h"

#define PI 3.14159265359

using namespace std;

void STFT(Eigen::VectorXf * signal, int signalLength, int windowSize, int hopSize);
void hanning(int windowLength, float* buffer);

int main() {

    return 0;

}

void STFT(Eigen::VectorXf* signal, int signalLength, int windowSize, int hopSize){


    fftwf_complex *data, *fft_result, *ifft_result;
    fftwf_plan plan_forward, plan_backward;
    int i;

    data = (fftwf_complex*) fftw_malloc(sizeof(fftw_complex) * windowSize);
    fft_result = (fftwf_complex*) fftw_malloc(sizeof(fftw_complex) * windowSize);
    ifft_result = (fftwf_complex*) fftw_malloc(sizeof(fftw_complex) * windowSize);

    plan_forward = fftwf_plan_dft_1d(windowSize, data, fft_result, FFTW_FORWARD, FFTW_ESTIMATE);

    float window[windowSize];
    hanning(windowSize, window);

    int chunkPosition = 0;
    int readIndex;
    int bStop = 0;
    int numChunks;

    while(chunkPosition < signalLength && !bStop){

        //copy chunk into buffer
        for(i = 0; i < windowSize; i++){

            readIndex = chunkPosition + i;

            if(readIndex < signalLength){

                data[i][0] = (*signal)[readIndex] * window[i];
                data[i][1] = 0.0;

            } else {

                data[i][0] = 0.0;
                data[i][1] = 0.0;
                bStop = 1;

            }
        }

        fftwf_execute(plan_forward);

        //Uncomment to see the raw-data output from the FFT calculation
        cout << "Column: " << chunkPosition << endl;
        for(i = 0 ; i < windowSize ; i++ ) {

            fprintf( stdout, "fft_result[%d] = { %2.2f, %2.2f }\n", i,
                     fft_result[i][0], fft_result[i][1]);

        }

        for (i = 0; i < windowSize/2 + 1; i++){


        }

        chunkPosition += hopSize;
        numChunks++;
    }
}

void hanning(int windowLength, float * buffer){

    for(int i = 0; i < windowLength; i++){

        buffer[i] = (float) 0.5 * (1 - cos(i*2*PI/(windowLength - 1)));

    }
}