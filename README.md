# BEMAD
A tool to predict whether a pulmonary nodule is benign or malignant according to blood cfDNA data.

## Compile the tool and prepare data

compile the tool

```
cd script
gcc -o pond_scale pond_scale.c -lm
cd ..
```

decompress the data

```
cd demo_data
tar -zxvf data.tar.gz
cd ..
```
Note:
1) Each line in the .train/.test files (in the folder demo_data) stands for the data of a sample. Numbers (separated by comma) in each line stand for the reads count for a 30K windows in the reference.
2) Each data file just include the data of more no then 10 samples, which is NOT enough to reproduce the results in the paper.

## Training

Search type-associated reads count patterns with a self-developed genetic algorithm

```
./script/pond_scale --input1 ./demo_data/data.txt.Cancer.train --input2 ./demo_data/data.txt.Benign.train --width 5 > ./demo_data/pond.txt
```
The number 5 means using continuous 5 windows.

## Predicting

Use patterns to predict samples (both training set and testing set)

```
perl ./script/getScore.-1.pl ./demo_data/data.txt.Cancer.train ./demo_data/pond.txt 10 > ./demo_data/score.Cancer.train.-1.txt
perl ./script/getScore.-1.pl ./demo_data/data.txt.Benign.train ./demo_data/pond.txt 10 > ./demo_data/score.Benign.train.-1.txt
perl ./script/getScore.-1.pl ./demo_data/data.txt.Cancer.test ./demo_data/pond.txt 10 > ./demo_data/score.Cancer.test.-1.txt
perl ./script/getScore.-1.pl ./demo_data/data.txt.Benign.test ./demo_data/pond.txt 10 > ./demo_data/score.Benign.test.-1.txt
perl ./script/getScore.pl ./demo_data/data.txt.Cancer.train ./demo_data/pond.txt 10 > ./demo_data/score.Cancer.train.txt
perl ./script/getScore.pl ./demo_data/data.txt.Benign.train ./demo_data/pond.txt 10 > ./demo_data/score.Benign.train.txt
perl ./script/getScore.pl ./demo_data/data.txt.Cancer.test ./demo_data/pond.txt 10 > ./demo_data/score.Cancer.test.txt
perl ./script/getScore.pl ./demo_data/data.txt.Benign.test ./demo_data/pond.txt 10 > ./demo_data/score.Benign.test.txt
```
The number 10 means using top 10 patterns for prediction.

Balance the scores obtained with benign-/maglinant- related patterns.

```
perl ./script/scale.pl ./demo_data/score.Benign.train.txt ./demo_data/score.Cancer.train.txt ./demo_data/score.Benign.train.-1.txt ./demo_data/score.Cancer.train.-1.txt ./demo_data/score.Benign.test.txt ./demo_data/score.Cancer.test.txt ./demo_data/score.Benign.test.-1.txt ./demo_data/score.Cancer.test.-1.txt 0 > scaled.score.txt
```

Now, we could calculate the AUC with the file scaled.score.txt.




