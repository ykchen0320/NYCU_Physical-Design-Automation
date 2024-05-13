#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
std::map<std::string, std::vector<int>> blocks, coordinate;
std::queue<std::pair<int, int>> free_queue;
float ratio_low, ratio_high, ratio = 0.0;
int area = 0, area_width = 0, area_height = 0, find_x = 0, find_y = 0;
std::string last_block;
void ReadFile(std::string rf) {
  std::ifstream myfile(rf);
  std::string str, temp;
  if (myfile.is_open()) {
    std::vector<std::string> v;
    int flag = 2;
    while (myfile.good()) {
      myfile >> str;
      if (flag) {
        if (flag == 2)
          ratio_low = stof(str);
        else if (flag == 1)
          ratio_high = stof(str);
        flag--;
      } else {
        if (str[0] == 'b') {
          temp = str;
          last_block = str;
        } else
          blocks[temp].push_back(stoi(str));
      }
    }
  } else
    std::cout << "Unable to open file";
  return;
}
void WriteFile(std::string wf) {
  std::ofstream myfile(wf);
  if (myfile.is_open()) {
    myfile << "A = " << area << std::endl;
    myfile << "R = " << ratio << std::endl;
    for (auto& it : coordinate) {
      myfile << it.first << " " << it.second[0] << " " << it.second[1]
             << std::endl;
    }
    myfile.close();
  } else
    std::cout << "Unable to open file";
  return;
}
bool Check_space(std::vector<int> block) {
  int i = find_x;
  for (; i < find_x + block[0]; i++) {
    if (i == find_x || i == find_x + block[0] - 1) {
      for (int j = find_y; j < find_y + block[1]; j++) {
        for (auto& it : coordinate) {
          if ((i > it.second[0]) && (i < it.second[0] + blocks[it.first][0])) {
            if ((j > it.second[1]) &&
                (j < it.second[1] + blocks[it.first][1])) {
              return false;
            }
          }
        }
      }
    } else {
      for (auto& it : coordinate) {
        if ((i > it.second[0]) && (i < it.second[0] + blocks[it.first][0])) {
          if ((find_y > it.second[1]) &&
              (find_y < it.second[1] + blocks[it.first][1])) {
            return false;
          }
        }
      }
    }
  }
  return true;
}
bool Find(std::vector<int> block) {
  if (Check_space(block)) {
    return true;
  } else
    return false;
}
void Initial() {
  free_queue.emplace(0, 0);
  for (auto& it : blocks) {
    while (!free_queue.empty()) {
      std::pair<int, int> temp_pair;
      temp_pair = free_queue.front();
      find_x = temp_pair.first;
      find_y = temp_pair.second;
      if (Find(it.second)) {
        free_queue.pop();
        coordinate[it.first].emplace_back(find_x);
        coordinate[it.first].emplace_back(find_y);
        free_queue.emplace(find_x + it.second[0], find_y);
        free_queue.emplace(find_x, find_y + it.second[1]);
        free_queue.emplace(find_x + it.second[0], find_y + it.second[1]);
        break;
      } else
        free_queue.pop();
    }
  }
}
void Count_ans() {
  for (auto& it : coordinate) {
    if (area_width < it.second[0] + blocks[it.first][0])
      area_width = it.second[0] + blocks[it.first][0];
    if (area_height < it.second[1] + blocks[it.first][1])
      area_height = it.second[1] + blocks[it.first][1];
  }
  area = area_height * area_width;
  ratio = ((float)area_width / (float)area_height);
}
void Fit_ratio() {
  find_x = area_width, find_y = area_height;
  if (ratio < ratio_low || ratio > ratio_high) {
    coordinate.erase(last_block);
    if (ratio < ratio_low)
      find_x = area_height * ratio_low;
    else
      find_y = area_width / ratio_high;
    find_x -= blocks[last_block][0];
    find_y -= blocks[last_block][1];
    while (!Check_space(blocks[last_block])) {
      find_x += blocks[last_block][0];
      find_y += blocks[last_block][1];
      find_x *= 1.5;
      find_y *= 1.5;
    }
    coordinate[last_block].emplace_back(find_x);
    coordinate[last_block].emplace_back(find_y);
  }
  Count_ans();
}
int main(int argc, char* argv[]) {
  ReadFile(argv[1]);
  Initial();
  Count_ans();
  Fit_ratio();
  WriteFile(argv[2]);
  return 0;
}
