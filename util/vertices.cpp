#include <iostream>
#include <vector>

int grid = 2;

int main(int argc, char *argv[])
{
	std::vector<float> vert;

	for(int j = 0; j < grid + 1; j++)
		for(int i = 0; i < grid + 2; i++) {
			for(int k = 0; k < ((i==0) ? 2 : 1); k++) {
				vert.push_back(float(i)/grid);
				vert.push_back(float(j)/grid);
				vert.push_back(0.f);
			}
			++j;
			for(int k = 0; k < ((i==grid+1) ? 2 : 1); k++) {
				vert.push_back(float(i)/grid);
				vert.push_back(float(j)/grid);
				vert.push_back(0.f);
			}
			--j;
		}

	for(std::vector<float>::iterator it = vert.begin(); it != vert.end(); it++)
		std::cout << *it << std::endl;

	return 0;
}
