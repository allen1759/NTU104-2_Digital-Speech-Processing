#!/bin/bash

source path.sh;

disambig -text Submit_Files/testdata/example.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/example.txt
disambig -text Submit_Files/testdata/1.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/1.txt
disambig -text Submit_Files/testdata/2.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/2.txt
disambig -text Submit_Files/testdata/3.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/3.txt
disambig -text Submit_Files/testdata/4.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/4.txt
disambig -text Submit_Files/testdata/5.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/5.txt
disambig -text Submit_Files/testdata/6.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/6.txt
disambig -text Submit_Files/testdata/7.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/7.txt
disambig -text Submit_Files/testdata/8.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/8.txt
disambig -text Submit_Files/testdata/9.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/9.txt
disambig -text Submit_Files/testdata/10.txt -map Submit_Files/ZhuYin-Big5.map -lm bigram.lm -order 2  > Submit_Files/result1/10.txt

echo Translate_DONE!

