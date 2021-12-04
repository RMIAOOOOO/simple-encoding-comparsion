#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

void huffmanDict (map<int, string> &encodingDictionary, const vector<int> &symbols, const vector<float> &probs) {
    
    encodingDictionary.clear();
    
    vector<pair<float, int> > probSymbolPair;
    priority_queue<pair<float, vector<int> > > probsSymbolsListPair;

    for (int i = 0; i < symbols.size(); ++i) {
        probSymbolPair.push_back(make_pair(probs[i], symbols[i]));
        vector<int> symbol = {symbols[i]};
        probsSymbolsListPair.push(make_pair(probs[i], symbol));
        encodingDictionary.insert({symbols[i], ""});
    }

    vector<pair<vector<int>, vector<int> > > mergeRecord;

    while(probsSymbolsListPair.size() >= 2) {
        pair<float, vector<int> > topPair = probsSymbolsListPair.top();
        probsSymbolsListPair.pop();
        pair<float, vector<int> > secondPair = probsSymbolsListPair.top();
        probsSymbolsListPair.pop();
        float summedProb = topPair.first + secondPair.first;
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

void huffmanTrial() {
    vector<int> symbols = {1,2,3,4,5,6};
    vector<float> probabilities = {0.1, 0.4, 0.06, 0.1, 0.04, 0.3};
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

void arithmeticEncode(string &resultString, int &encodeLength, const vector<int> &inputString, const vector<int> &symbols, const vector<float> &probabilities){
    map<int, float> preCumulativeProbabilities;
    map<int, float> postCumulativeProbabilities;
    float probSum = 0;
    for(int i = 0; i < probabilities.size(); ++i) {
        preCumulativeProbabilities[symbols[i]]= probSum;
        probSum += probabilities[i];
        postCumulativeProbabilities[symbols[i]]=probSum;
    }
    float encodedLowerBound = 0;
    float encodedUpperBound = 1;
    for(int i = 0; i < inputString.size(); ++i) {
        float encodeRange = encodedUpperBound - encodedLowerBound;
        encodedUpperBound = encodedLowerBound + postCumulativeProbabilities[inputString[i]] * encodeRange;
        encodedLowerBound += preCumulativeProbabilities[inputString[i]] * encodeRange;
    }

    resultString = "";
    float resultLowerBound = 0;
    float resultUpperBound = 1;
    while(1) {
        float resultMidPoint = (resultLowerBound + resultUpperBound) / 2;
        if (resultUpperBound <= encodedUpperBound && resultLowerBound >= encodedLowerBound) {
            break;
        } else if (resultMidPoint < encodedLowerBound) {
            resultLowerBound = resultMidPoint;
            resultString += "1";
        } else if (resultMidPoint > encodedUpperBound) {
            resultUpperBound = resultMidPoint;
            resultString += "0";
        } else if (encodedLowerBound < resultMidPoint && resultMidPoint < encodedUpperBound) {
            float lowerBoundDiff = resultMidPoint - encodedLowerBound;
            float upperBoundDiff = encodedUpperBound - resultMidPoint;
            if (lowerBoundDiff > upperBoundDiff) {
                resultUpperBound = resultMidPoint;
                resultString += "0";
            } else {
                resultLowerBound = resultMidPoint;
                resultString += "1";
            }
        } else {
            cout << "error at arithmetic encoding\n";
        }
    }
    encodeLength = inputString.size();
}

void arithmeticDecode(vector<int> &result, string inputCode, const vector<int> &symbols, const vector<float> &probabilities, int encodeLength) {
    float encodeLowerBound = 0;
    float encodeUpperBound = 1;
    for(int i = 0; i < inputCode.length(); ++i) {
        float encodeRange = encodeUpperBound - encodeLowerBound;
        if (inputCode[i] == '1') {
            encodeLowerBound += encodeRange / 2;
        } else {
            encodeUpperBound -= encodeRange / 2;
        }
    }
    
    map<int, float> preCumulativeProbabilities;
    map<int, float> postCumulativeProbabilities;
    float probSum = 0;
    for(int i = 0; i < probabilities.size(); ++i) {
        preCumulativeProbabilities[symbols[i]]= probSum;
        probSum += probabilities[i];
        postCumulativeProbabilities[symbols[i]]=probSum;
    }

    result.resize(0);
    float scannerLowerBound = 0;
    float scannerUpperBound = 1;
    for(int i = 0; i < encodeLength; ++i) {
        float scannerRange = scannerUpperBound - scannerLowerBound;
        for (int i = 0; i < symbols.size(); ++i) {
            float lowerBound = scannerLowerBound + scannerRange * preCumulativeProbabilities[symbols[i]];
            float upperBound = scannerLowerBound + scannerRange * postCumulativeProbabilities[symbols[i]];
            if (lowerBound <= encodeLowerBound && upperBound >= encodeUpperBound) {
                result.push_back(symbols[i]);
                scannerLowerBound = lowerBound;
                scannerUpperBound = upperBound;
                break;
            }
        }
    }
}

void arithmeticTrial() {
    vector<int> symbols = {1,2,3};
    vector<float> probabilities = {0.5, 0.33, 0.17};

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

int main(void) {

    // huffmanTrial();
    arithmeticTrial();

    return 0;
}