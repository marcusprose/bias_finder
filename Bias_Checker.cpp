/* A program that will read in a file (a newspaper or article of some sort)
and outputting a bias percentage that indicates how biased a news article is.

Citation: Special thanks to Sentiment-Lexicon
  (Subjectivity Lexicon)
  Recognizing Contextual Polarity in Phrase-Level Sentiment Analysis
  Theresa Wilson, Janyce Wiebe and Paul Hoffmann
*/

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

using namespace std;
void store_data(ifstream &infile, vector<string> *file_data);
bool check_adv(vector<string> *file_data, int i);
bool check_adj(vector<string> *file_data, int i);
bool check_modal(vector<string> *file_data, int i);
bool check_percent(vector<string> *file_data, int i);
bool check_sym(vector<string> *file_data, int i);
bool check_contract(vector<string> *file_data, int i);
bool check_num(vector<string> *file_data, int i);
bool check_personal(vector<string> *file_data, int i);

int main(int argc, char *argv[])
{

    ifstream infile;
    string file;
    if (argc == 2){
        infile.open(argv[1]);
    } else {
        cerr << "Please try again with: './executable file_name'" << endl;
        return 0;
    }
    if (!infile){
        cerr << "Error: unable to open file" << endl;
        exit(EXIT_FAILURE);
    }
    vector<string> file_data;
    store_data(infile, &file_data);
    int adv_count, adj_count, sym_count, percent_count, contract_count,
    num_count, personal_count, modal_count = 0;
    float total_count = 0;
    for (unsigned long i = 0; i < file_data.size(); i++){
        if (check_adv(&file_data, i)) {
            adv_count++;
        }
        if (check_adj(&file_data, i)) {
            adj_count++;
        }
        if (check_sym(&file_data, i)) {
          sym_count++;
        }
        if (check_num(&file_data, i)){
            num_count++;
        }
        if (check_percent(&file_data, i)) {
          percent_count++;
        }
        if (check_contract(&file_data, i)) {
          contract_count++;
        }
        if (check_personal(&file_data, i)) {
          personal_count++;
        }
        if (check_modal(&file_data, i)){
          modal_count++;
        }
    }
    // change the scaling once you have a working system
    total_count = (adv_count*550) + (adj_count*225) + (sym_count*650) +
      (modal_count*700) + (personal_count*900) + (contract_count*400)
      - (percent_count*500) - (num_count*250);

    cout << "Average_bias: " << total_count/file_data.size() << endl;
    //showing data
    cout << "adv: " << adv_count << " adj: " << adj_count << endl;
    cout << "sym: " << sym_count << " modal: " << modal_count << endl;
    cout << "personal: " << personal_count << " contract: " << contract_count << endl;
    cout << "[minus] percentages: " << percent_count << " num: " << num_count << endl;
    // one more scaling
    infile.close();

    return 0;
}

void store_data(ifstream &infile, vector<string> *file_data){
    string word;
    while (infile >> word){
        file_data->push_back(word);
    }
}

bool check_adv(vector<string> *file_data, int i)
{
    string word = file_data->at(i);
    int n = word.length();
    if (n >= 4 && word.substr(n-2, n) == "ly")
    {
      return true;
    }
  return false;
}

bool check_adj(vector<string> *file_data, int i)
{
    vector<string> list_adj;
    ifstream infile;
    infile.open("list_of_adj.txt");
    store_data(infile, &list_adj);
    for (unsigned long a = 0; a < list_adj.size(); a++){
        if (file_data->at(i) == list_adj.at(a)){
            return true;
        }
    }
    return false;
}

bool check_modal(vector<string> *file_data, int i)
{
  vector<string> list_modal;
  ifstream infile;
  infile.open("list_of_modal.txt");
  store_data(infile, &list_modal);
  for (unsigned long a = 0; a < list_modal.size(); a++){
      if (file_data->at(i) == list_modal.at(a)){
          infile.close();
          return true;
      }
  }
  infile.close();
  return false;
}

bool check_personal(vector<string> *file_data, int i)
{
  vector<string> list_pp;
  ifstream infile;
  infile.open("list_of_pp.txt");
  store_data(infile, &list_pp);
  for (unsigned long a = 0; a < list_pp.size(); a++){
      if (file_data->at(i) == list_pp.at(a)){
          infile.close();
          return true;
      }
  }
  infile.close();
  return false;
}

bool check_sym(vector<string> *file_data, int i)
{
    string word = file_data->at(i);
    char sym = word[word.length() - 1];
    if (sym == ('?') || sym == '!')
    {
      return true;
    }
  return false;
}

bool check_percent(vector<string> *file_data, int i)
{
  string word = file_data->at(i);
  char percent = word[word.length() - 1];
  if (percent == '%')
  {
    return true;
  }

  return false;
}

bool check_num(vector<string> *file_data, int i)
{
  string word = file_data->at(i);
  if (isdigit(word[0])){
    return true;
  }
  return false;
}

bool check_contract(vector<string> *file_data, int i){
  string word = file_data->at(i);
  char apostrophe = word[word.length() - 2];
  if (apostrophe == '\'')
  {
    if (!isupper(word[0]))
    {
      return true;

    }
  }
  return false;
}
