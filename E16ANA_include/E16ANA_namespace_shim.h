#ifndef E16ANA_NAMESPACE_SHIM_H
#define E16ANA_NAMESPACE_SHIM_H

// 1. Standard C++ streams support
#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::ios;

// 2. ROOT utility functions support (e.g. Form)
#include <TString.h>

// 3. ROOT TSpectrum 6 compatibility wrapper (float* -> double*)
#include <TSpectrum.h>
#include <vector>

class MockTSpectrum : public ::TSpectrum {
public:
   MockTSpectrum() : ::TSpectrum() {}
   MockTSpectrum(Int_t maxpositions, Double_t resolution = 1) : ::TSpectrum(maxpositions, resolution) {}
   
   // Overload Deconvolution to accept float* and const float*
   Int_t Deconvolution(float *source, const float *response, Int_t ssize, Int_t numberIterations, Int_t numberRepetitions, Double_t boost) {
      std::vector<double> d_source(ssize);
      std::vector<double> d_response(ssize);
      for (int i = 0; i < ssize; ++i) {
         d_source[i] = source[i];
         d_response[i] = response[i];
      }
      ::TSpectrum::Deconvolution(d_source.data(), d_response.data(), ssize, numberIterations, numberRepetitions, boost);
      for (int i = 0; i < ssize; ++i) {
         source[i] = d_source[i];
      }
      return 0;
   }
   
   // Overload DeconvolutionRL to accept float* and const float*
   Int_t DeconvolutionRL(float *source, const float *response, Int_t ssize, Int_t numberIterations, Int_t numberRepetitions, Double_t boost) {
      std::vector<double> d_source(ssize);
      std::vector<double> d_response(ssize);
      for (int i = 0; i < ssize; ++i) {
         d_source[i] = source[i];
         d_response[i] = response[i];
      }
      ::TSpectrum::DeconvolutionRL(d_source.data(), d_response.data(), ssize, numberIterations, numberRepetitions, boost);
      for (int i = 0; i < ssize; ++i) {
         source[i] = d_source[i];
      }
      return 0;
   }
};

#define TSpectrum MockTSpectrum

// 4. Geant4/CLHEP System of Units support (e.g. mm, GeV)
#include <G4SystemOfUnits.hh>
using namespace CLHEP;

#endif // E16ANA_NAMESPACE_SHIM_H
