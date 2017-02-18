explanation: speller.c will use the functionality in dictionary.c to check the words in your provided file
against the provided (or default) dictionary, returning output to the console.

directory: pset5/speller

to compile: $ make

to run: Usage: speller [dictionary] text
using default dictionary: $ ./speller texts/aaadummy.txt
or to use your own dictionary: $ ./speller dictionaries/small texts/aaadummy.txt


Currently, the only file that works (without a seg fault) is aaadummy.txt; others are apparently too large
So run:  $ ./speller texts/aaadummy.txt