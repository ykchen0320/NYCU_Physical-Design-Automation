#include <algorithm>
#include <ctime>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
float r, low, high, ans_cutsize, cutsize1;
int Pmax, G1, G2, cells_amount, ans_G1, ans_G2;
std::unordered_map<std::string, std::vector<std::string>> nets_map, cells_map;
std::unordered_map<std::string, int> ans_cell, cell1;
std::unordered_map<std::string, int> gain_value;
std::unordered_map<std::string, bool> visits;
std::unordered_map<int, std::forward_list<std::string>> gain_map;
void ReadFile(std::string rf) {
  std::ifstream myfile(rf);
  std::string str, temp;
  if (myfile.is_open()) {
    std::vector<std::string> v;
    while (myfile.good()) {
      myfile >> str;
      if (r == 2.0) {
        r = stof(str);
        continue;
      }
      if (str == ";") {
        std::sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
        nets_map[temp] = v;
        v.clear();
        temp = "";
      }
      if (str[0] == 'n') temp = str;
      if (str[0] == 'c') v.push_back(str);
    }
  } else
    std::cout << "Unable to open file";
  if (nets_map.find("") != nets_map.end()) nets_map.erase("");
  return;
}
void WriteFile(std::string wf) {
  std::vector<std::string> ans_vG1, ans_vG2;
  for (auto& it : ans_cell) {
    if (it.second) {
      ans_vG2.push_back(it.first);
    } else {
      ans_vG1.push_back(it.first);
    }
  }
  std::ofstream myfile(wf);
  if (myfile.is_open()) {
    myfile << "Cutsize = " << ans_cutsize << std::endl;
    myfile << "G1 " << ans_vG1.size() << std::endl;
    for (int i = 0; i < ans_vG1.size(); i++) {
      myfile << ans_vG1[i] << " ";
      if (i == ans_vG1.size() - 1) myfile << ";" << std::endl;
    }
    myfile << "G2 " << ans_vG2.size() << std::endl;
    for (int i = 0; i < ans_vG2.size(); i++) {
      myfile << ans_vG2[i] << " ";
      if (i == ans_vG2.size() - 1) myfile << ";" << std::endl;
    }
    myfile.close();
  } else
    std::cout << "Unable to open file";
  return;
}
void Make_cells_map() {
  int temp = 0;
  Pmax = 0;
  for (auto& it : nets_map) {
    for (int i = 0; i < it.second.size(); i++) {
      if (cells_map.find(it.second[i]) == cells_map.end())
        visits[it.second[i]] = true;
      if (gain_value.find(it.second[i]) == gain_value.end())
        gain_value[it.second[i]] = 0;
      if (std::find(cells_map[it.second[i]].begin(),
                    cells_map[it.second[i]].end(),
                    it.first) == cells_map[it.second[i]].end())
        cells_map[it.second[i]].push_back(it.first);
      temp = cells_map[it.second[i]].size();
      Pmax = std::max(Pmax, temp);
    }
  }
  return;
}
bool Balance(int temp_G1, int temp_G2) {
  if ((low <= temp_G1) && (high >= temp_G1) && (low <= temp_G2) &&
      (high >= temp_G2))
    return true;
  else
    return false;
}
void Initial_Gain() {
  int left, right;
  std::string s_left, s_right;
  for (auto& it : nets_map) {
    left = 0, right = 0;
    std::string s_left, s_right;
    for (int i = 0; i < it.second.size(); i++) {
      if (cell1[it.second[i]]) {
        right++;
        s_right = it.second[i];
      } else {
        left++;
        s_left = it.second[i];
      }
    }
    if (left == 1)
      gain_value[s_left]++;
    else if (left == 0) {
      for (int i = 0; i < it.second.size(); i++) gain_value[it.second[i]]--;
    }
    if (right == 1)
      gain_value[s_right]++;
    else if (right == 0) {
      for (int i = 0; i < it.second.size(); i++) gain_value[it.second[i]]--;
    }
  }
  for (auto& it : gain_value) gain_map[it.second].emplace_front(it.first);
  return;
}
void First_Cut() {
  cutsize1 = 0;
  int left, right;
  for (auto& it : nets_map) {
    left = 0, right = 0;
    for (int i = 0; i < it.second.size(); i++) {
      if (cell1[it.second[i]] == 1)
        right++;
      else
        left++;
      if (left && right) {
        cutsize1++;
        break;
      }
    }
  }
  ans_cutsize = cutsize1;
  return;
}
void Initial_Partition() {
  srand(time(NULL));
  int x;
  G1 = 0, G2 = 0;
  for (auto& it : cells_map) {
    x = rand() % 2;
    cells_amount++;
    low = float((float)(cells_amount * 0.5) - (int)(r * cells_amount * 0.5));
    high = float((float)(cells_amount * 0.5) + (int)(r * cells_amount * 0.5));
    if (x) {
      if (Balance(G1, G2 + 1)) {
        G2++;
        cell1[it.first] = 1;
      } else {
        G1++;
        cell1[it.first] = 0;
      }
    } else {
      if (Balance(G1 + 1, G2)) {
        G1++;
        cell1[it.first] = 0;
      } else {
        G2++;
        cell1[it.first] = 1;
      }
    }
  }
  ans_cell = cell1;
  ans_G1 = G1;
  ans_G2 = G2;
  Initial_Gain();
  First_Cut();
  ans_cutsize = cutsize1;
  return;
}
void Update_Gain(std::string node) {
  int left, right, flag = cell1[node];
  cutsize1 -= gain_value[node];
  gain_value.erase(node);
  std::forward_list<std::string> modify_list;
  for (auto& it : cells_map[node]) {
    std::string s_left, s_right;
    left = 0, right = 0;
    for (auto& it2 : nets_map[it]) {
      if (cell1[it2]) {
        right++;
        if (it2 != node) s_right = it2;
      } else {
        left++;
        if (it2 != node) s_left = it2;
      }
    }
    visits[node] = false;
    if (flag) {
      if (left == 0) {
        for (auto& it2 : nets_map[it]) {
          for (auto& it3 : modify_list) {
            if (it3 != it2) {
              modify_list.emplace_front(it3);
              gain_map[gain_value[it3]].remove(it3);
            }
          }
          if (visits[it2]) gain_value[it2]++;
        }
      } else if (left == 1) {
        for (auto& it3 : modify_list) {
          if (it3 != s_left) {
            modify_list.emplace_front(it3);
            gain_map[gain_value[it3]].remove(it3);
          }
        }
        if (visits[s_left]) gain_value[s_left]--;
      }
      left++;
      right--;
      if (right == 0) {
        for (auto& it2 : nets_map[it]) {
          for (auto& it3 : modify_list) {
            if (it3 != it2) {
              modify_list.emplace_front(it3);
              gain_map[gain_value[it3]].remove(it3);
            }
          }
          if (visits[it2]) gain_value[it2]--;
        }
      } else if (right == 1) {
        for (auto& it3 : modify_list) {
          if (it3 != s_right) {
            modify_list.emplace_front(it3);
            gain_map[gain_value[it3]].remove(it3);
          }
        }
        if (visits[s_right]) gain_value[s_right]++;
      }
    } else {
      if (right == 0) {
        for (auto& it2 : nets_map[it]) {
          for (auto& it3 : modify_list) {
            if (it3 != it2) {
              modify_list.emplace_front(it3);
              gain_map[gain_value[it3]].remove(it3);
            }
          }
          if (visits[it2]) gain_value[it2]++;
        }
      } else if (right == 1) {
        for (auto& it3 : modify_list) {
          if (it3 != s_right) {
            modify_list.emplace_front(it3);
            gain_map[gain_value[it3]].remove(it3);
          }
        }
        if (visits[s_right]) gain_value[s_right]--;
      }
      left--;
      right++;
      if (left == 0) {
        for (auto& it2 : nets_map[it]) {
          for (auto& it3 : modify_list) {
            if (it3 != it2) {
              modify_list.emplace_front(it3);
              gain_map[gain_value[it3]].remove(it3);
            }
          }
          if (visits[it2]) gain_value[it2]--;
        }
      } else if (left == 1) {
        for (auto& it3 : modify_list) {
          if (it3 != s_left) {
            modify_list.emplace_front(it3);
            gain_map[gain_value[it3]].remove(it3);
          }
        }
        if (visits[s_left]) gain_value[s_left]++;
      }
    }
  }
  if (flag) {
    G1++;
    G2--;
    cell1[node] = 0;
  } else {
    G2++;
    G1--;
    cell1[node] = 1;
  }
  for (auto& it : modify_list) gain_map[gain_value[it]].emplace_front(it);
}
void Update() {
  if (cutsize1 < ans_cutsize) {
    ans_cutsize = cutsize1;
    ans_cell = cell1;
    ans_G1 = G1;
    ans_G2 = G2;
  }
}
void Find() {
  bool boo = false;
  for (int i = Pmax; i >= -Pmax; i--) {
    if (gain_map.find(i) != gain_map.end()) {
      for (auto& it : gain_map[i]) {
        if (cell1[it])
          boo = Balance(G1 + 1, G2 - 1);
        else
          boo = Balance(G1 - 1, G2 + 1);
        if (boo) {
          Update_Gain(it);
          Update();
          break;
        }
      }
    }
    if (boo) break;
  }
}
void FM() {
  Make_cells_map();
  Initial_Partition();
  for (auto& it : cell1) Find();
}
int main(int argc, char* argv[]) {
  r = 2.0;
  ReadFile(argv[1]);
  FM();
  WriteFile(argv[2]);
  return 0;
}
