#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

void huffmanDict (map<int, string> &encodingDictionary, const vector<int> &symbols, const vector<double> &probs) {
    
    encodingDictionary.clear();
    
    vector<pair<double, int> > probSymbolPair;
    priority_queue<pair<double, vector<int> > > probsSymbolsListPair;

    for (int i = 0; i < symbols.size(); ++i) {
        probSymbolPair.push_back(make_pair(probs[i], symbols[i]));
        vector<int> symbol = {symbols[i]};
        probsSymbolsListPair.push(make_pair(probs[i], symbol));
        encodingDictionary.insert({symbols[i], ""});
    }

    vector<pair<vector<int>, vector<int> > > mergeRecord;

    while(probsSymbolsListPair.size() >= 2) {
        pair<double, vector<int> > topPair = probsSymbolsListPair.top();
        probsSymbolsListPair.pop();
        pair<double, vector<int> > secondPair = probsSymbolsListPair.top();
        probsSymbolsListPair.pop();
        double summedProb = topPair.first + secondPair.first;
        vector<int> firstList(topPair.second.begin(), topPair.second.end());
        vector<int> secondList(secondPair.second.begin(), secondPair.second.end());
        mergeRecord.push_back(make_pair(firstList, secondList));
        
        vector<int> mergedList(firstList.begin(), firstList.end());
        mergedList.insert(mergedList.end(), secondList.begin(), secondList.end());
        probsSymbolsListPair.push(make_pair(summedProb, mergedList));
    }

    for (int i = mergeRecord.size()-1 ; i >=0 ; --i) {
        vector<int> bigProbSymbolVector = mergeRecord[i].first;
        vector<int> smallProbSymbolVector = mergeRecord[i].second;
        for (int j = 0; j < bigProbSymbolVector.size(); ++j) {
            encodingDictionary[bigProbSymbolVector[j]] = encodingDictionary[bigProbSymbolVector[j]] + "0";
        }
        for (int j = 0; j < smallProbSymbolVector.size(); ++j) {
            encodingDictionary[smallProbSymbolVector[j]] = encodingDictionary[smallProbSymbolVector[j]] + "1";
        }
    }
}

string huffmanEncode(const vector<int> &inputString, const map<int, string> &dictionary) {
    string result = "";
    for (int i = 0; i < inputString.size(); ++i) {
        result += dictionary.at(inputString[i]);
    }
    return result;
}

void huffmanDecode(vector<int> &result, string inputCode, const map<int, string> &dictionary) {
    result.resize(0);
    map<string, int> codeToValueMap;
    
    for (auto it = dictionary.begin(); it != dictionary.end(); it++)
    {
        int value = it->first;
        string code = it->second;
        codeToValueMap[code]=value;
    }

    while(inputCode.length() > 0) {
        int checkingLength = 1;
        while(true) {
            string currentCheckString = inputCode.substr(0, checkingLength);
            auto it = codeToValueMap.find(currentCheckString);
            if(it != codeToValueMap.end()) {
                result.push_back(it->second);
                inputCode = inputCode.substr(checkingLength, inputCode.length()-checkingLength);
                break;
            } else {
                checkingLength ++;
            }
        }
    }
}

void checkInputOutputConsistent(const vector<int> &original, const vector<int> &reverted) {
    if (original.size() != reverted.size()) {
        cout << "error: encode decode size is not consistent!\n";
    } else {
        for (int i = 0 ; i < original.size(); ++i) {
            if (original[i] != reverted[i]) {
                cout << "error; encode decode data is not consistent!\n";
            }
        }  
    }
}

int huffmanTrial(const vector<double> &probabilities, const vector<int> &inputs) {
    
    vector<int> symbols(probabilities.size(), 0);
    for (int i = 0; i < symbols.size(); ++i) symbols[i] = i+1;
    map<int, string> huffmanEncodingDictionary;
    huffmanDict(huffmanEncodingDictionary, symbols, probabilities);
    string huffmanEncodedString = huffmanEncode(inputs, huffmanEncodingDictionary);
    vector<int> decodedHuffmanCode;
    huffmanDecode(decodedHuffmanCode, huffmanEncodedString, huffmanEncodingDictionary);
    checkInputOutputConsistent(inputs, decodedHuffmanCode);
    return huffmanEncodedString.length();
}

void arithmeticEncode(string &resultString, int &encodeLength, const vector<int> &inputString, const vector<int> &symbols, const vector<double> &probabilities){
    // cout << inputString << "\n";
    // for (int i = 0; i < inputString.size(); ++i) {
    //     cout << inputString[i] << " ";
    // }
    // cout << "\n";
    map<int, double> preCumulativeProbabilities;
    map<int, double> postCumulativeProbabilities;
    double probSum = 0;
    for(int i = 0; i < probabilities.size(); ++i) {
        preCumulativeProbabilities[symbols[i]]= probSum;
        probSum += probabilities[i];
        postCumulativeProbabilities[symbols[i]]=probSum;
    }
    double encodedLowerBound = 0;
    double encodedUpperBound = 1;
    for(int i = 0; i < inputString.size(); ++i) {
        double encodeRange = encodedUpperBound - encodedLowerBound;
        encodedUpperBound = encodedLowerBound + postCumulativeProbabilities[inputString[i]] * encodeRange;
        encodedLowerBound += preCumulativeProbabilities[inputString[i]] * encodeRange;
    }

    resultString = "";
    double resultLowerBound = 0;
    double resultUpperBound = 1;
    while(1) {
        double resultMidPoint = (resultLowerBound + resultUpperBound) / 2;
        if (resultUpperBound <= encodedUpperBound && resultLowerBound >= encodedLowerBound) {
            break;
        } else if (resultMidPoint <= encodedLowerBound) {
            resultLowerBound = resultMidPoint;
            resultString += "1";
        } else if (resultMidPoint >= encodedUpperBound) {
            resultUpperBound = resultMidPoint;
            resultString += "0";
        } else if (encodedLowerBound <= resultMidPoint && resultMidPoint <= encodedUpperBound) {
            double lowerBoundDiff = resultMidPoint - encodedLowerBound;
            double upperBoundDiff = encodedUpperBound - resultMidPoint;
            if (lowerBoundDiff > upperBoundDiff) {
                resultUpperBound = resultMidPoint;
                resultString += "0";
            } else {
                resultLowerBound = resultMidPoint;
                resultString += "1";
            }
        } else {
            cout << "error at arithmetic encoding\n";
            cout << "encodedLowerBound" << " " << "resultLowerBound" << " " << "resultMidPoint" << " " << "resultUpperBound" << " " << "encodedUpperBound" << "\n";
            cout << encodedLowerBound << " " << resultLowerBound << " " << resultMidPoint << " " << resultUpperBound << " " << encodedUpperBound << "\n";
            break;
        }
    }
    encodeLength = inputString.size();
}

void arithmeticDecode(vector<int> &result, string inputCode, const vector<int> &symbols, const vector<double> &probabilities, int encodeLength) {
    double encodeLowerBound = 0;
    double encodeUpperBound = 1;
    for(int i = 0; i < inputCode.length(); ++i) {
        double encodeRange = encodeUpperBound - encodeLowerBound;
        if (inputCode[i] == '1') {
            encodeLowerBound += encodeRange / 2;
        } else {
            encodeUpperBound -= encodeRange / 2;
        }
    }
    
    map<int, double> preCumulativeProbabilities;
    map<int, double> postCumulativeProbabilities;
    double probSum = 0;
    for(int i = 0; i < probabilities.size(); ++i) {
        preCumulativeProbabilities[symbols[i]]= probSum;
        probSum += probabilities[i];
        postCumulativeProbabilities[symbols[i]]=probSum;
    }

    result.resize(0);
    double scannerLowerBound = 0;
    double scannerUpperBound = 1;
    for(int i = 0; i < encodeLength; ++i) {
        double scannerRange = scannerUpperBound - scannerLowerBound;
        for (int i = 0; i < symbols.size(); ++i) {
            double lowerBound = scannerLowerBound + scannerRange * preCumulativeProbabilities[symbols[i]];
            double upperBound = scannerLowerBound + scannerRange * postCumulativeProbabilities[symbols[i]];
            if (lowerBound <= encodeLowerBound && upperBound >= encodeUpperBound) {
                result.push_back(symbols[i]);
                scannerLowerBound = lowerBound;
                scannerUpperBound = upperBound;
                break;
            }
        }
    }
}

int arithmeticTrial(const vector<double> &probabilities, const vector<int> &inputs) {
    
    vector<int> symbols(probabilities.size(), 0);
    for (int i = 0; i < symbols.size(); ++i) symbols[i] = i+1;

    string arithmeticEncodedString;
    int arithmeticInputLength = 0;
    arithmeticEncode(arithmeticEncodedString, arithmeticInputLength, inputs, symbols, probabilities);
    vector<int> decodedArithmeticCode;
    arithmeticDecode(decodedArithmeticCode, arithmeticEncodedString, symbols, probabilities, arithmeticInputLength);
    checkInputOutputConsistent(inputs, decodedArithmeticCode);
    return arithmeticEncodedString.length();
}

void generateWordDistribution(vector<double> &resultProbabilities, int inputVariety) {
    srand(time(NULL));
    resultProbabilities.resize(0);

    vector<double> rawdoubles;
    for (int i = 0; i < inputVariety-1; ++i) {
        int randInt = rand();
        double randdouble = static_cast <double> (randInt) / static_cast <double> (RAND_MAX);
        rawdoubles.push_back(randdouble);
    }
    sort(rawdoubles.begin(), rawdoubles.end());

    resultProbabilities.resize(0);
    resultProbabilities.push_back(rawdoubles[0]);
    for (int i = 1 ; i < rawdoubles.size(); ++i) {
        resultProbabilities.push_back(rawdoubles[i] - rawdoubles[i-1]);
    }
    resultProbabilities.push_back(1 - rawdoubles[rawdoubles.size()-1]);
}

void generateInput(vector<int> &resultInput, const vector<double> &probabilities, int wordLength) {
    srand(time(NULL));
    resultInput.resize(0);
    int inputRange = probabilities.size();
    
    for (int i = 0; i < wordLength; ++i) {
        int randInt = rand();
        double randdouble = static_cast <double> (randInt) / static_cast <double> (RAND_MAX);
        for (int j = 0; j < probabilities.size(); ++j) {
            double currentProb = probabilities[j];
            if (randdouble <= currentProb) {
                resultInput.push_back(j+1);
                break;
            }
            randdouble -= currentProb;
        }
    }
}

void debugMain() {
    // huffman
    {
        vector<int> symbols = {1,2,3,4,5,6};
        vector<double> probabilities = {0.1, 0.4, 0.06, 0.1, 0.04, 0.3};
        map<int, string> huffmanEncodingDictionary;
        huffmanDict(huffmanEncodingDictionary, symbols, probabilities);


        cout << "\n" << "Huffman Encoded Dictionary" << "\n";
        cout << "Symbol" << "\t" << "Encoding" << "\n";
        for(auto it = huffmanEncodingDictionary.cbegin(); it != huffmanEncodingDictionary.cend(); ++it) {
            cout << it->first << "\t" << it->second << "\n";
        }
        vector<int> inputString = {2,5,1};
        string huffmanEncodedString = huffmanEncode(inputString, huffmanEncodingDictionary);
        cout << "Input string: ";
        for (int i = 0; i < inputString.size(); ++i) cout << inputString[i];
        cout << ", Encoded string: " << huffmanEncodedString;
        vector<int> decodedHuffmanCode;
        huffmanDecode(decodedHuffmanCode, huffmanEncodedString, huffmanEncodingDictionary);
        cout << ", Decoded string: ";
        for (int i = 0; i < inputString.size(); ++i) cout << decodedHuffmanCode[i];
        cout << "\n";
    }
    
    // arithmetic
    {
        vector<int> symbols = {1,2,3};
        vector<double> probabilities = {0.5, 0.33, 0.17};

        vector<int> inputString = {2,3,1};
        string arithmeticEncodedString;
        int arithmeticInputLength = 0;
        arithmeticEncode(arithmeticEncodedString, arithmeticInputLength, inputString, symbols, probabilities);
        cout << "Input string: ";
        for (int i = 0; i < inputString.size(); ++i) cout << inputString[i];
        cout << ", Encoded string: " << arithmeticEncodedString;
        vector<int> decodedArithmeticCode;
        arithmeticDecode(decodedArithmeticCode, arithmeticEncodedString, symbols, probabilities, arithmeticInputLength);
        cout << ", Decoded string: ";
        for (int i = 0; i < inputString.size(); ++i) cout << decodedArithmeticCode[i];
        cout << "\n";
    }
}

int main(void) {
    const int INPUT_VARIETY_ITER = 4;
    const int PROBABILITIES_GENERATION_ITER = 100;
    const int WORD_LENGTH_ITER = 3;
    const int INPUT_GENERATION_ITER = 100;
    vector<vector<int> > huffmanVarietyLengthCumulativeLength(INPUT_VARIETY_ITER, vector<int>(WORD_LENGTH_ITER, 0));
    vector<vector<int> > arithmeticVarietyLengthCumulativeLength(INPUT_VARIETY_ITER, vector<int>(WORD_LENGTH_ITER, 0));

    for (int inputVarietyIteration = 0; inputVarietyIteration < INPUT_VARIETY_ITER; inputVarietyIteration ++) {
        for (int probabilitiesGenerateIteration = 0; probabilitiesGenerateIteration < PROBABILITIES_GENERATION_ITER; ++ probabilitiesGenerateIteration) {
            vector<double> probabilities;
            int inputVariety = 1 << (inputVarietyIteration) + 1;
            generateWordDistribution(probabilities, inputVariety);
            for (int wordLengthIteration = 0; wordLengthIteration < WORD_LENGTH_ITER; wordLengthIteration ++) {
                int wordLength = 1 << (wordLengthIteration + 1);
                for (int inputGenerateIteration = 0; inputGenerateIteration < INPUT_GENERATION_ITER; ++ inputGenerateIteration) {
                    vector<int> generatedInput;
                    generateInput(generatedInput, probabilities, wordLength);
                    int encodedLength = huffmanTrial(probabilities, generatedInput);
                    huffmanVarietyLengthCumulativeLength[inputVarietyIteration][wordLengthIteration] += huffmanTrial(probabilities, generatedInput);
                    arithmeticVarietyLengthCumulativeLength[inputVarietyIteration][wordLengthIteration] += arithmeticTrial(probabilities, generatedInput);
                }
            }
        }
    }

    cout << "Huffman\n";
    cout << "len/var\t";
    for (int inputVarietyIteration = 0; inputVarietyIteration < INPUT_VARIETY_ITER; ++inputVarietyIteration) {
        cout << (1 << (inputVarietyIteration + 1)) << "\t"; 
    }
    cout << "\n";
    for (int wordLengthIteration = 0; wordLengthIteration < WORD_LENGTH_ITER; ++wordLengthIteration) {
        cout << (1 << (wordLengthIteration + 1)) << "\t";
        for (int inputVarietyIteration = 0; inputVarietyIteration < INPUT_VARIETY_ITER; ++inputVarietyIteration) {
            double totalLength = huffmanVarietyLengthCumulativeLength[inputVarietyIteration][wordLengthIteration];
            double avgLength = totalLength / PROBABILITIES_GENERATION_ITER / INPUT_GENERATION_ITER;
            cout << avgLength  << "\t"; 
        }
        cout << "\n";
    }

    cout << "Arithmetic\n";
    cout << "len/var\t";
    for (int inputVarietyIteration = 0; inputVarietyIteration < INPUT_VARIETY_ITER; ++inputVarietyIteration) {
        cout << (1 << (inputVarietyIteration + 1)) << "\t"; 
    }
    cout << "\n";
    for (int wordLengthIteration = 0; wordLengthIteration < WORD_LENGTH_ITER; ++wordLengthIteration) {
        cout << (1 << (wordLengthIteration + 1)) << "\t";
        for (int inputVarietyIteration = 0; inputVarietyIteration < INPUT_VARIETY_ITER; ++inputVarietyIteration) {
            double totalLength = arithmeticVarietyLengthCumulativeLength[inputVarietyIteration][wordLengthIteration];
            double avgLength = totalLength / PROBABILITIES_GENERATION_ITER / INPUT_GENERATION_ITER;
            cout << avgLength  << "\t"; 
        }
        cout << "\n";
    }
    return 0;
}