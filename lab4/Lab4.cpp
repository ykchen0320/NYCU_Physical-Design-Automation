#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
std::map<std::string, std::vector<int>> T_list, B_list;
std::vector<int> T_net, B_net;
std::map<int, std::pair<int, int>> net_list;
std::map<int, std::vector<int>> constraint, track;
std::map<int, int> constraint_count;
int t = 0;
void ReadFile(std::string rf) {
  std::ifstream myfile(rf);
  std::string str, temp;
  std::vector<int> temp_vec;
  int count = 0;
  while (myfile.good()) {
    myfile >> str;
    if (count) {
      count--;
      if (temp[0] == 'T') {
        T_list[temp].emplace_back(stoi(str));
      } else if (temp[0] == 'B') {
        B_list[temp].emplace_back(stoi(str));
      }
    } else {
      if (str[0] == 'T' || str[0] == 'B') {
        temp = str;
        count = 2;
      } else {
        temp_vec.emplace_back(stoi(str));
      }
    }
  }
  int temp_vec_size = temp_vec.size();
  for (int i = 0; i < temp_vec_size; i++) {
    if (i < (temp_vec_size / 2))
      T_net.emplace_back(temp_vec[i]);
    else
      B_net.emplace_back(temp_vec[i]);
  }
  return;
}
void WriteFile(std::string wf) {
  std::ofstream myfile(wf);
  myfile << "Channel density: " << t + 1;
  for (auto &it : track) {
    myfile << std::endl << "Net " << it.first;
    myfile << std::endl
           << "C" << t - it.second[0] + 1 << " " << it.second[1] << " "
           << it.second[2];
  }
  myfile.close();
  return;
}
void Create_net_list() {
  for (int i = 0; i < T_net.size(); i++) {
    if (net_list.find(T_net[i]) == net_list.end())
      net_list[T_net[i]] = std::make_pair(i, i);
    else
      net_list[T_net[i]].second = i;
    if (net_list.find(B_net[i]) == net_list.end())
      net_list[B_net[i]] = std::make_pair(i, i);
    else
      net_list[B_net[i]].second = i;
  }
  net_list.erase(0);
}
void Create_constraint_list() {
  for (int i = 0; i < T_net.size(); i++) {
    if (T_net[i] && B_net[i]) constraint[T_net[i]].emplace_back(B_net[i]);
    if (constraint_count.find(T_net[i]) == constraint_count.end())
      constraint_count[T_net[i]] = 0;
    if (constraint_count.find(B_net[i]) == constraint_count.end())
      constraint_count[B_net[i]] = 0;
    if (T_net[i] && B_net[i]) constraint_count[B_net[i]]++;
  }
  constraint_count.erase(0);
}
void Constraint_cleaner(int clean_num) {
  if (constraint.find(clean_num) != constraint.end()) {
    for (auto &it : constraint[clean_num]) {
      constraint_count[it]--;
    }
    constraint.erase(clean_num);
  }
  return;
}
void Route() {
  int watermark = -1;
  while (net_list.size()) {
    int temp = -1;
    for (auto &it : net_list) {
      if (constraint_count[it.first] || it.second.first <= watermark) continue;
      if (temp == -1)
        temp = it.first;
      else if (net_list[temp].first > it.second.first)
        temp = it.first;
    }
    if (temp == -1) {
      watermark = -1;
      t++;
    } else {
      watermark = net_list[temp].second;
      track[temp].emplace_back(t);
      track[temp].emplace_back(net_list[temp].first);
      track[temp].emplace_back(net_list[temp].second);
      Constraint_cleaner(temp);
      net_list.erase(temp);
    }
  }
}
int main(int argc, char *argv[]) {
  ReadFile(argv[1]);
  Create_net_list();
  Create_constraint_list();
  Route();
  WriteFile(argv[2]);
  return 0;
}