# 15-Band Graphic Equalizer (JUCE)

A simple **15-band graphic equalizer** built with [JUCE](https://juce.com/).  
It covers the audible spectrum in ~2/3-octave steps, with each band controlled by a vertical slider for intuitive boosting and cutting.

## Features
- 15 fixed frequency bands spanning the audible spectrum  
- Real-time stereo audio processing  
- Peaking filters implemented with JUCE’s `IIRFilter`  
- Automation and preset recall via `AudioProcessorValueTreeState`  
- Simple, intuitive GUI with frequency-labeled sliders  

## Overview
This plugin processes stereo audio in real time, applies user-defined gain adjustments per band, and outputs the modified signal.  
While intentionally minimal, it’s a practical tool for experimenting with:
- EQ design concepts  
- Filter coefficient implementation  
- DAW integration and automation  

## Screenshot
![15 Band Graphic EQ](./15%20Band%20Graphic%20EQ.png)

## Disclaimer
This plugin is primarily designed for learning and experimentation.  
It’s not optimized for the highest fidelity but serves as a solid educational resource.
