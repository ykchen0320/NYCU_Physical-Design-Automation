#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
std::vector<std::vector<std::string>> pmos_instances, nmos_instances, pmos_pins,
    nmos_pins;
std::unordered_map<std::string, std::vector<std::string>> cell_list, pmos_list,
    nmos_list;
std::unordered_map<std::string, std::vector<std::pair<float, float>>>
    coordinate;
std::unordered_map<std::string, float> pins_length;
float nmos_w, pmos_w, HPWL = 0, l = 20;
void ReadFile(std::string rf) {
  std::ifstream myfile(rf);
  std::string str;
  std::string temp;
  int count;
  while (myfile.good()) {
    myfile >> str;
    if (str == ".ENDS") continue;
    if (str == ".SUBCKT" || (str[0] == 'M' && str[1] == 'M')) {
      temp = str.substr(1);
      count = 0;
      continue;
    } else {
      if (temp == "SUBCKT") {
        count++;
        if (count == 1) continue;
      }
      if (temp[0] == 'M') {
        count++;
        if (count > 5) {
          if (str[0] == 'w') {
            if (cell_list[temp][4] == "nmos_rvt") {
              nmos_w = stof(str.substr(2, str.size() - 3));
            } else
              pmos_w = stof(str.substr(2, str.size() - 3));
          }
          continue;
        }
      }
      cell_list[temp].emplace_back(str);
    }
  }
  for (auto& it : cell_list) {
    if (it.second[4] == "pmos_rvt") {
      pmos_list[it.first].emplace_back(it.second[0]);
      pmos_list[it.first].emplace_back(it.second[1]);
      pmos_list[it.first].emplace_back(it.second[2]);
    } else if (it.second[4] == "nmos_rvt") {
      nmos_list[it.first].emplace_back(it.second[0]);
      nmos_list[it.first].emplace_back(it.second[1]);
      nmos_list[it.first].emplace_back(it.second[2]);
    }
  }
  return;
}
void WriteFile(std::string wf) {
  std::ofstream myfile(wf);
  myfile << HPWL << std::endl;
  int space_count = 0;
  for (auto& it : pmos_instances) {
    for (int i = 0; i < it.size(); i++) {
      if (space_count++) myfile << " ";
      myfile << it[i];
    }
  }
  space_count = 0;
  myfile << std::endl;
  for (auto& it : pmos_pins) {
    for (int i = 0; i < it.size(); i++) {
      if (space_count++) myfile << " ";
      myfile << it[i];
    }
  }
  space_count = 0;
  myfile << std::endl;
  for (auto& it : nmos_instances) {
    for (int i = 0; i < it.size(); i++) {
      if (space_count++) myfile << " ";
      myfile << it[i];
    }
  }
  space_count = 0;
  myfile << std::endl;
  for (auto& it : nmos_pins) {
    for (int i = 0; i < it.size(); i++) {
      if (space_count++) myfile << " ";
      myfile << it[i];
    }
  }
  myfile << std::endl;
  myfile.close();
  return;
}
void Initial() {
  std::vector<std::string> elist;
  std::string temp1, temp2;
  for (auto& it1 : pmos_list) {
    temp1 = it1.first;
    for (auto& it2 : nmos_list) {
      if (it2.second[1] == it1.second[1]) {
        temp2 = it2.first;
        break;
      }
    }
    std::vector<std::string> instances_temp1, pins_temp1, instances_temp2,
        pins_temp2;
    instances_temp1.emplace_back(temp1);
    instances_temp1.emplace_back("Dummy");
    instances_temp2.emplace_back(temp2);
    instances_temp2.emplace_back("Dummy");
    for (int i = 0; i < pmos_list[temp1].size(); i++) {
      pins_temp1.emplace_back(pmos_list[temp1][i]);
      pins_temp2.emplace_back(nmos_list[temp2][i]);
    }
    pins_temp1.emplace_back("Dummy");
    pins_temp2.emplace_back("Dummy");
    pmos_instances.emplace_back(instances_temp1);
    nmos_instances.emplace_back(instances_temp2);
    pmos_pins.emplace_back(pins_temp1);
    nmos_pins.emplace_back(pins_temp2);
    elist.emplace_back(temp1);
    nmos_list.erase(temp2);
  }
  pmos_instances[pmos_instances.size() - 1].pop_back();
  nmos_instances[nmos_instances.size() - 1].pop_back();
  pmos_pins[pmos_pins.size() - 1].pop_back();
  nmos_pins[nmos_pins.size() - 1].pop_back();
  for (auto& it : elist) {
    pmos_list.erase(it);
  }
}
void Count_coordinate() {
  float x = 0, pheight = (nmos_w / 2) + 27 + (pmos_w / 2);
  int count = 0;
  for (int i = 0; i < pmos_pins.size(); i++) {
    for (int j = 0; j < pmos_pins[i].size(); j++) {
      if (count++ % 2) {
        if (pmos_pins[i][j] == "Dummy") {
          x += (l + l + 34);
          continue;
        } else {
          x += l;
          continue;
        }
      }
      if ((i == 0 && j == 0) ||
          ((i == pmos_pins.size() - 1) && (j == pmos_pins[i].size() - 1))) {
        x += 12.5;
        coordinate[pmos_pins[i][j]].emplace_back(x, pheight);
        x += 12.5;
      } else {
        x += 17;
        coordinate[pmos_pins[i][j]].emplace_back(x, pheight);
        x += 17;
      }
    }
  }
  count = 0, x = 0;
  for (int i = 0; i < nmos_pins.size(); i++) {
    for (int j = 0; j < nmos_pins[i].size(); j++) {
      if (count++ % 2) {
        if (nmos_pins[i][j] == "Dummy") {
          x += (l + l + 34);
          continue;
        } else {
          x += l;
          continue;
        }
      }
      if (nmos_pins[i][j] == "Dummy") {
        x += (l + l + 34);
        continue;
      }
      if ((i == 0 && j == 0) ||
          ((i == nmos_pins.size() - 1) && (j == nmos_pins[i].size() - 1))) {
        x += 12.5;
        coordinate[nmos_pins[i][j]].emplace_back(x, 0);
        x += 12.5;
      } else {
        x += 17;
        coordinate[nmos_pins[i][j]].emplace_back(x, 0);
        x += 17;
      }
    }
  }
}
void Count_pins_length() {
  for (auto& it : coordinate) {
    float x_min = it.second[0].first, x_max = it.second[0].first,
          y_max = it.second[0].second, y_min = it.second[0].second;
    for (int i = 0; i < it.second.size(); i++) {
      if (it.second[i].first > x_max) x_max = it.second[i].first;
      if (it.second[i].first < x_min) x_min = it.second[i].first;
      if (it.second[i].second > y_max) y_max = it.second[i].second;
      if (it.second[i].second < y_min) y_min = it.second[i].second;
    }
    pins_length[it.first] = (x_max - x_min) + (y_max - y_min);
  }
}
void Count_HPWL() {
  Count_coordinate();
  Count_pins_length();
  for (auto& it : pins_length) {
    HPWL += it.second;
  }
}
int main(int argc, char* argv[]) {
  ReadFile(argv[1]);
  Initial();
  Count_HPWL();
  WriteFile(argv[2]);
  return 0;
}