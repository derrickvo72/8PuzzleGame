#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <fstream>
#include<array>

using namespace std;
#define SIZE 3


typedef struct States
{
	int A[3][3];
	int empty = 0;//O:rong ; 1: khong rong
} State;


State Goal = { {{0, 1, 2},{3, 4, 5},{6, 7, 8}}, 0 }; 

//012
//345
//678
double randomPercent(double start, double end)
{
	random_device pd;
	mt19937 device(pd());
	uniform_real_distribution<double> about(start, end);
	return about(device);
}
class Game
{
public:
	State initiate;

	Game() {}
	Game(State input)
	{
		//Xử lí chuối nhập
		initiate = input;
	}
	void print() {
		printf("*-----*-----*-----*\n");
		for (int i = 0; i < SIZE; i++) {
			cout << "|";
			for (int j = 0; j < SIZE; j++)
				printf(" _%d_ |", initiate.A[i][j]);

			cout << endl;
		}
		printf("*-----*-----*-----*\n");
	}
	int PathCost() {
		if (initiate.empty == 0)
			return 1000;
		

		// Khoảng cách ô lệch vị trí ->Manhatan
		int value = 0;

		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
			{
			if (initiate.A[i][j] == 0)
			continue;
			int x, y;
			for (x = 0; x < SIZE; x++)
			for (y = 0; y < SIZE; y++)
			//So sánh vị trí ô muốn di chuyển đến
			if (initiate.A[i][j] == Goal.A[x][y])
			goto step;
			step:// Tính khoảng cách ô initiate cho đến Goal
				value += abs(x - i) + abs(y - j);
			}


		return value;
	}
	//Xét vị trí trạng thái ô trống và hướng di chuyển xung quanh bằng 
	vector<Game> getDirection() {
		vector<Game> Moving;

		int i, j;
		for (i = 0; i < SIZE; i++)
			for (j = 0; j < SIZE; j++)
				if (initiate.A[i][j] == 0)
					goto step;
	step:
		//Lên
		if (i != 0)
		{
			Game MoveTravel(initiate);
			swap(MoveTravel.initiate.A[i][j], MoveTravel.initiate.A[i - 1][j]);
			Moving.push_back(MoveTravel);
		}
		//Xuống
		if (i != SIZE - 1)
		{
			Game MoveTravel(initiate);
			swap(MoveTravel.initiate.A[i][j], MoveTravel.initiate.A[i + 1][j]);
			Moving.push_back(MoveTravel);
		}
		//Trái
		if (j != 0)
		{
			Game MoveTravel(initiate);
			swap(MoveTravel.initiate.A[i][j], MoveTravel.initiate.A[i][j - 1]);
			Moving.push_back(MoveTravel);
		}
		//Phải
		if (j != SIZE - 1)
		{
			Game MoveTravel(initiate);
			swap(MoveTravel.initiate.A[i][j], MoveTravel.initiate.A[i][j + 1]);
			Moving.push_back(MoveTravel);
		}


		return Moving;
	}
	//Lấy giá trị bé value nhỏ nhất ->Heuristic
	Game MovingBest() {
		int Min = 1000;
		vector<Game> compare;
		//So sánh chi phí để lấy State
		vector<Game> Moving = getDirection();
		for (int i = 0; i < Moving.size(); i++)
			if (Moving[i].PathCost() < Min)
				Min = Moving[i].PathCost();
		for (int i = 0; i < Moving.size(); i++)
			if (Moving[i].PathCost() == Min)
				compare.push_back(Moving[i]);
		return compare[rand() % compare.size()];
	}

	Game RandomDirect()
	{
		int Min = 1000;
		vector<Game> compare;
		//So sánh chi phí để lấy State
		vector<Game> Moving = getDirection();
		for (int i = 0; i < Moving.size(); i++)
			if (Moving[i].PathCost() < Min)
				Min = Moving[i].PathCost();
		for (int i = 0; i < Moving.size(); i++)
			if (Moving[i].PathCost() == Min)
				compare.push_back(Moving[i]);
		return compare[rand() % compare.size()];
	}
	//Radom hàng xóm xung quanh với chi phí bé hơn hoặc bằng sẽ lấy
	Game RamdomDirection() {
		Game random;
		do 
		{
		random = getDirection()[rand() % getDirection().size()];
		} while (random.initiate.empty == 0);// nếu vecto con rỗng
		//lấy trang 1 trong số các initiate cận kề được radom
		return random;
	}
	//Check đã đạt trạng thái đích // số ô lệch với vị trí ban đầu
	bool CheckReward() {
		if (PathCost() == 0)
		return true;
		return false;
	}
};

Game HillClimbing(Game puzzle, int& solve) { //HillClimbing
	if (puzzle.CheckReward())
	{
		solve = 0;
		return puzzle;
	}
	Game current = puzzle;
	int maxMove = 25000;
	for (int i = 0; i < maxMove; i++) {
		Game neighbor = current.MovingBest();
		if (neighbor.PathCost() > current.PathCost())
			return current;
		current = neighbor;
		solve = i;
	}
	return current;
}
//Game FirstChoice_HillClimbing(Game puzzle, int& solve) {
//	Game current = puzzle;
//	for (int i = 0; i < 150; i++) {
//		if (current.CheckReward())
//		{
//			solve = i;
//			return current;
//		}
//		int count = 0;
//		Game random;
//		do {
//			random = current.RamdomDirection();
//			if (random.PathCost() < current.PathCost())
//				break;
//			count++;
//			if (count == 20)
//				random = current;
//		} while (count != 20);
//		current = random;
//		solve = i;
//	}
//	return current;
//}

//Game Random_HillClimbingSearch(Game puzzle) {
//
//	if (puzzle.CheckReward())
//		return puzzle;
//	Game current = puzzle;
//
//	Game neighbor = current.MovingBest();
//	if (neighbor.PathCost() > current.PathCost())
//		return current;
//	current = neighbor;
//	return current;
//
//}
Game RandomRestart_HillClimbing(Game puzzle, int& solve) // RandomRestart
{
	Game Temp;
	int i = 0;
	do
	{
		solve = i + 1;
		if (i != 0)
		{
			int n = randomPercent(1, 10);
			for (int i = 0; i < n; i++)
			{
				puzzle = puzzle.RamdomDirection();

			}
		}
		else
			//Temp = Random_HillClimbingSearch(puzzle);
			Temp = HillClimbing(puzzle, solve);
		i++;
	} while (Temp.PathCost() != 0 && i < 1000);
	return Temp;
}

Game Simulatedannealing(Game puzzle, int& solve)
{
	Game current;
	current = puzzle;
	double Temperature = 100; //Cho Nhiệt độ ban đầu là 100 độ để nung
	for (int i = 1; i <= 300; i++) //Giới hạn số lần shake mỗi khi giảm nhiệt độ
	{
		solve = i;
		if (current.CheckReward()) {
			return current;
		}
		Temperature *= 0.96; //giảm nhiệt độ
		
		Game next = current.RandomDirect();//MaxPing
		int deltaE = next.PathCost() - current.PathCost(); //Tính delta
		if (deltaE > 0) {
			current = next;

		}
		else
		{
			float e = exp(deltaE / Temperature); // pow(e,deltaE / Temperature) //deltaE càng lớn thì e càng lớn xs càng nhỏ càng tốt , càng lớn càng dở
			double rand = randomPercent(0.0, 1.0); //Radom xs 
			if (e>rand) {
				current =next;

			}
		}
	}
			return current;
}
int is_equal(int a[3][3], int b[3][3]) //kiểm tra bằng nhau 2 mảng
{
	int i, j, flag = 1;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			if (a[i][j] != b[i][j])
				flag = 0;
	return flag;
}

bool trungAA(Game aa[50], int n, Game temp) //kiểm tra trùng 2 state
{
	for (int i = 0; i < n; i++)
	{
		if (is_equal(aa[i].initiate.A, temp.initiate.A))
		{
			return true;
		}
	}
	return false;
}
int main()
{

	cout << "*---------------------Subject: AI -------------------*" << endl;
	cout << "              Project: Game 8 Puzzle" << endl << endl;
	cout << "+Instructor: MSIT.Huynh Xuan Phung" << endl << endl;
	cout << "+Student:" << endl;
	cout << "#MSSV:\t\t\t#Name:" << endl;
	cout << "18110101\t\tVo Phu Duc" << endl << endl;
	cout << "18110075\t\tHa Duc Anh" << endl << endl;
	cout << "*------------------Year: 2019-2020 ------------------*" << endl << endl;



	Game aa[50]; //Mảng chứa 50 trạng thái 
	////Game inputState({ {2,1,4},{3,6,0},{7,8,5} });
	cout << "Goal State:" << endl;
	Game inputState({ {{0, 1, 2},{3, 4, 5},{6, 7, 8}}, 1 });
	inputState.print();
	cout << "========================================================" << endl << endl;

	//aa[0] =inputState;
	//Game inputState2({ {{0, 1, 2},{3, 4, 5},{6, 7, 8}}, 1 });
	//aa[1]= inputState;
	//Game inputState3({ {{3,1,2},{6,0,5},{7,4,8}},1 });
	//aa[2]=inputState3;
	//Game inputState4({ {{3,2,1},{0,4,6},{5,7,8} }, 1 });
	//aa[3]= inputState4;
	/*Game inputState5({ {3,1,5},{2,7,6},{4,0,8} });
	aa.push_back(inputState5);
	Game inputState6({ {2,3,1},{5,4,6},{7,0,8} });
	aa.push_back(inputState6);
	Game inputState7({ {3,1,2},{6,4,5},{0,7,8} });
	aa.push_back(inputState7);
	Game inputState8({ {3,1,7},{5,4,6},{2,0,8} });
	aa.push_back(inputState8);
	Game inputState9({ {3,1,5},{2,4,6},{7,0,8} });
	aa.push_back(inputState9);
	Game inputState10({ {3,1,2},{5,4,6},{0,7,8} });
	aa.push_back(inputState10);
	Game inputState11({ {3,1,2},{4,5,6},{7,0,8} });
	aa.push_back(inputState11);
	Game inputState12({ {2,1,3},{5,4,6},{7,0,8} });
	aa.push_back(inputState12);*/
	for (int i = 1; i <= 20; i++) //Radom 50 trnagj thái ban đầu
	{
		//cout << "Start State " << i << endl;
		Game game;
		do
		{
			int n = randomPercent(1, 20);
			Game temp({ {{0, 1, 2},{3, 4, 5},{6, 7, 8}}, 1 }); //Goal
			for (int i = 0; i < n; i++)
			{
				temp = temp.RamdomDirection();
			}
			game = temp;
		} while (trungAA(aa, i + 1, game));

		aa[i] = game;

		//aa[i].print();
	}


#pragma region MAIN MENU
	//Menu thuật toán
	cout << "*-----------------> MENU ALGORITH <-----------------*\n";
	cout << "|=>CHOOSE  |                                        |\n";
	cout << "|     0    |  EXIT !                                |\n";
	cout << "|----------|----------------------------------------|\n";
	cout << "|     1    |  Hill climbing                         |\n";
	cout << "|----------|----------------------------------------|\n";
	cout << "|     2    |  Hill climbing with random restart     |\n";
	cout << "|----------|----------------------------------------|\n";
	cout << "|     3    |  Simulated annealing                   |\n";
	cout << "*---------------------------------------------------*\n";
	cout << "\n";
	int q;

	cout << "Your choose: ";
	do
	{
		do
		{
			cin >> q;
			if (q < 0 || q>6)
			{
				printf("\n0<=q<=5\n");
			}

		} while (q < 0 || q  >6);
		Game solution;
		if (q == 0) //Ấn 0 để thoát
		{
			exit(0);
		}
		if (q == 1)// Thuật toán leo đồi
		{
			/*	printf("+Hill climbing->steepest-ascent\n\n");
				clock_t start = clock();
				 solution = Steepest_HillClimbing(inputState);
				solution.print();
				printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
				printf("Path value is: ");
				cout << solution.PathCost();*/
			printf("+Hill climbing\n\n");
			clock_t start = clock();
			int n = 0;
			double time = 0;
			for (int i = 1; i <= 20; i++)
			{
				clock_t start = clock();
				int solve;
				solution = HillClimbing(aa[i], solve);

				if (i >= 10) { cout << "State_" << i << "    " << "CheckDone: " << solution.PathCost() << " \t"; }
				else
				{
					cout << "State_" << i << "     " << "CheckDone: " << solution.PathCost() << " \t";
				}


				cout << "Step: " << solve << " ";

				if (solution.PathCost() == 0)
				{
					n++;
				}
				time += ((double)(clock() - start) / CLOCKS_PER_SEC);
				if (solve >= 10) { printf("        Time: %.3fs\n", (double)(clock() - start) / CLOCKS_PER_SEC); }
				else { printf("         Time: %.3fs\n", (double)(clock() - start) / CLOCKS_PER_SEC); }



			}
			cout << "=>Number of solve: ";
			cout << n << endl;
			printf("Time: %.3fs\n", ((double)(clock() - start) / CLOCKS_PER_SEC) );
			//Thoi gian trung binh;
			printf("\nTimeAverage: %.6fs\n", time / 20);
		}

		//if (q == 2)
		//{
		//	clock_t start = clock();
		//	printf("+Hill climbing->first-choice variant\n\n");
		//	clock_t start1 = clock();
		//	int n = 0;
		//	for (int i = 1; i <= 50; i++)
		//	{
		//		int solve2;
		//		//solution = FirstChoice_HillClimbing(aa[i], solve2);
		//		cout << "State_" << i << "     " << "CheckDone: " << solution.PathCost() << " \t";
		//		//cout << "Step: " << solve2;
		//		if (solution.PathCost() == 0)
		//		{
		//			n++;
		//		}
		//		printf("         Time: %.2fs\n", (double)(clock() - start1) / CLOCKS_PER_SEC);
		//	}
		//	cout << "=>Number of solve: ";
		//	cout << n << endl;
		//	printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
		//}


		if (q == 2)//Thuật toán radom restart
		{
			clock_t start = clock();
			printf("+Hill climbing with random restart\n\n");

			int n = 0;
			double time = 0;
			for (int i = 1; i <= 20; i++)
			{
				clock_t start1 = clock();
				int solve3;
				solution = RandomRestart_HillClimbing(aa[i], solve3);
				if (i >= 10) { cout << "State_" << i << "    " << "CheckDone: " << solution.PathCost() << " \t"; }
				else { cout << "State_" << i << "     " << "CheckDone: " << solution.PathCost() << " \t"; }
				cout << "Step: " << solve3;
				if (solution.PathCost() == 0)
				{
					n++;
				}
				if (solve3 >= 10 && solve3 <= 999) { printf("            Time: %.3fs\n", (double)(clock() - start1) / CLOCKS_PER_SEC); }
				else if (solve3 == 1000) { printf("          Time: %.3fs\n", (double)(clock() - start1) / CLOCKS_PER_SEC); }
				else { printf("             Time: %.3fs\n", (double)(clock() - start1) / CLOCKS_PER_SEC); }
				time = time + (double)(clock() - start1) / CLOCKS_PER_SEC;
			}
				cout << "=>Number of solve: ";
				cout << n << endl;
				printf("Time: %.3fs\n", ((double)(clock() - start) / CLOCKS_PER_SEC) );
				//Thoi gian trung binh;
				printf("\nTimeAverage: %.6fs\n", time / 20);
			}
			if (q == 3) //Thuật toán nung 
			{
				clock_t start = clock();
				printf("+Simulated annealing\n\n");
				int n = 0;
				double time = 0;
				for (int i = 1; i <= 20; i++)
				{
					clock_t start1 = clock();
					int solve4;
					solution = Simulatedannealing(aa[i], solve4);
					if (i >= 10) { cout << "State_" << i << "    " << "CheckDone: " << solution.PathCost() << " \t"; }
					else { cout << "State_" << i << "     " << "CheckDone: " << solution.PathCost() << " \t"; }


					cout << "Step: " << solve4;

					if (solution.PathCost() == 0)
					{
						n++;
					}

					if (solve4 >= 10) { printf("        Time: %.3fs\n", (double)(clock() - start1) / CLOCKS_PER_SEC); }
					else { printf("         Time: %.3fs\n", (double)(clock() - start1) / CLOCKS_PER_SEC); }
					time = time + (double)(clock() - start1) / CLOCKS_PER_SEC;
				}
				cout << "=>Number of solve: ";
				cout << n << endl;
				printf("Time: %.3fs\n", ((double)(clock() - start) / CLOCKS_PER_SEC) );
				//Thoi gian trung binh;
				printf("\nTimeAverage: %.6fs\n", time / 20);
			}
			cout << endl << "-----------------------------------------------------------" << endl;

	} while (q <= 5);

#pragma endregion
		return 0;
}

