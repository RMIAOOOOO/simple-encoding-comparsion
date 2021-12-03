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

int main(void) {
    vector<int> symbols = {1,2,3,4,5,6};
    vector<float> probabilities = {0.1, 0.4, 0.06, 0.1, 0.04, 0.3};
    map<int, string> huffmanEncodingDictionary;
    huffmanDict(huffmanEncodingDictionary, symbols, probabilities);


    cout << "\n" << "Huffman Encoded Dictionary" << "\n";
    cout << "Symbol" << "\t" << "Encoding" << "\n";
    for(auto it = huffmanEncodingDictionary.cbegin(); it != huffmanEncodingDictionary.cend(); ++it) {
        cout << it->first << "\t" << it->second << "\n";
    }


    return 0;
}