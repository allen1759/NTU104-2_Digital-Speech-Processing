#!/bin/bash

source path.sh;

cd Submit_Files;mkdir testdata;cd ..

perl separator_big5.pl corpus.txt > corpus_seg.txt
perl separator_big5.pl testdata/example.txt > Submit_Files/testdata/example.txt
perl separator_big5.pl testdata/1.txt > Submit_Files/testdata/1.txt
perl separator_big5.pl testdata/2.txt > Submit_Files/testdata/2.txt
perl separator_big5.pl testdata/3.txt > Submit_Files/testdata/3.txt
perl separator_big5.pl testdata/4.txt > Submit_Files/testdata/4.txt
perl separator_big5.pl testdata/5.txt > Submit_Files/testdata/5.txt
perl separator_big5.pl testdata/6.txt > Submit_Files/testdata/6.txt
perl separator_big5.pl testdata/7.txt > Submit_Files/testdata/7.txt
perl separator_big5.pl testdata/8.txt > Submit_Files/testdata/8.txt
perl separator_big5.pl testdata/9.txt > Submit_Files/testdata/9.txt
perl separator_big5.pl testdata/10.txt > Submit_Files/testdata/10.txt
echo Perl_DONE!

ngram-count -text corpus_seg.txt -write lm.cnt -order 2
ngram-count -read lm.cnt -lm Submit_Files/bigram.lm -unk -order 2
echo ngram-count_DONE!

cd Submit_Files;make map; cd ..
echo MakeMap_DONE!
