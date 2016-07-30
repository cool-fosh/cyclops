/*
Copyright (c) Jon Newman (jpnewman ~at~ mit <dot> edu)
All right reserved.

This file is part of the Cyclops Library (CL) for Arduino.

CL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CL.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Cyclops.h>

#include "MySources.h"

cyclops::Board myb(cyclops::board::CH0);
cyclops::Waveform waveform(&myb, &gen);

void on_trigger() {

  Serial.println("Triggered.");
  waveform.source_ptr->reset();

}

void setup()
{
  pinMode(30, INPUT_PULLUP);
  pinMode(31, INPUT_PULLUP);
  pinMode(32, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);

  Serial.begin(57600);
  SPI_fifo.begin(SPI_CLOCK_16MHz); // 16MHz SPI clock, using pin 10 as CS
  
  double delta = cyclops::Waveform::initAll();
  Timer1.initialize(delta);
  Timer1.attachInterrupt(cyclops::cyclops_timer_isr); // Defined in Waveform_t.h

  myb.uponExternalTrigger(on_trigger, RISING);

}

void loop()
{
  cyclops::Waveform::processAll();
}

