#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ostream>
#include <stdio.h>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

enum action { H, S, D, P };
std::ostream& operator<<(std::ostream& s, const action t)
{
	switch(t) {
		case H : {
			cout << "[ Hit! ]";
			break;
		}
		case S : {
			cout << "[ Stand... ]";
			break;
		}
		case D : {
			cout << "[ Double x2 ]";
			break;
		}
		case P : {
			cout << "[ Split || ]";
			break;
		}
	}
    return s;
}

/*! Different basic strategies according to 
 ** http://info-graphic.ru/_nw/6/79801123.png */

class Strategy 
{
	public:
		Strategy() {}
		~Strategy() {}
		virtual action use(void) = 0;
};

class PairStrategy : public Strategy 
{
	int pair_strategy[10][10] = {
		{H, H, H, H, P, P, P, P, P, P},
		{H, H, H, H, P, P, P, P, P, P},
		{H, H, H, H, H, P, P, H, H, H},
		{H, H, D, D, D, D, D, D, D, D},
		{H, H, H, H, H, H, H, H, H, H},
		{H, H, H, H, P, P, P, P, P, P},
		{P, P, P, P, P, P, P, P, P, P},
		{S, S, P, P, S, P, P, P, P, P},
		{S, S, S, S, S, S, S, S, S, S},
		{P, P, P, P, P, P, P, P, P, P}
	};
	int first, second, dealer_card;

	public:
		PairStrategy(int first, int second, int dealer_card) : first(first), second(second), dealer_card(dealer_card) {}
		
		action use(void) 
		{
			cout << "It is pair: " << first << "-" << second << endl;
			int y = first - 2; int x = 11 - dealer_card;
			return (action) pair_strategy[y][x];
		}
};

class SoftStrategy : public Strategy 
{
	int soft_strategy[8][10] = {
		{H, H, H, H, H, D, D, H, H, H},
		{H, H, H, H, H, D, D, H, H, H},
		{H, H, H, H, H, D, D, D, H, H},
		{H, H, H, H, H, D, D, D, H, H},
		{H, H, H, H, H, D, D, D, D, H},
		{H, H, H, S, S, D, D, D, D, D},
		{S, S, S, S, S, D, D, D, D, D},
		{S, S, S, S, S, D, D, D, D, D},
	};
	int min, dealer_card;

	public:
		SoftStrategy(int min, int dealer_card) : min(min), dealer_card(dealer_card) {}
		
		action use(void) 
		{
			cout << "It is soft hand: A-" << min << endl;
			int y = min - 2; int x = 11 - dealer_card;
			return (action) soft_strategy[y][x];
		}
};

class HardStrategy : public Strategy 
{
	int hard_strategy[10][10] = {
		{H, H, H, H, H, H, H, H, H, H},
		{H, H, H, H, H, D, D, D, D, H},
		{H, H, D, D, D, D, D, D, D, D},
		{H, D, D, D, D, D, D, D, D, D},
		{H, H, H, H, H, S, S, S, H, H},
		{H, H, H, H, H, S, S, S, S, S},
		{H, H, H, H, H, S, S, S, S, S},
		{H, H, H, H, H, S, S, S, S, S},
		{H, H, H, H, H, S, S, S, S, S},
		{H, H, H, H, H, S, S, S, S, S},
	};
	int sum, dealer_card;

	public:
		HardStrategy(int sum, int dealer_card) : sum(sum), dealer_card(dealer_card) {}
		
		action use(void) 
		{
			cout << "It is hard hand, sum is " << sum << endl;
			if(sum < 8) sum = 8;
			if(sum > 17) sum = 17;
			int y = sum - 8; int x = 11 - dealer_card;
			return (action) hard_strategy[y][x];
		}
};

class Context
{
	protected:
		Strategy * operation;
 
	public:
		Context(void) {}
		~Context(void) {}
	 
		virtual action UseStrategy() = 0;
		virtual void SetStrategy(Strategy * v) = 0;
};

class Client: public Context
{
	public:
		Client(void) {}
		~Client(void) {}
	 
		action UseStrategy() { return operation->use(); }
		void SetStrategy(Strategy * o) { operation = o; }
};

class Player : public Client
{
	public:
		action play_with_dealer(int, int, int);
};

/*! Compute an action of the player according to basic strategy to play with dealer */
action Player::play_with_dealer (int first, int second, int dealer_card) {
	
	cout << "Dealer have: " << dealer_card << " X" << endl;
	cout << "Player have: " << first << " " << second << endl;

	int min = (first < second) ? first : second;
	int sum = first + second;

	bool isPair = (first == second);
	bool isSoft = (first == 11 || second == 11) && (min < 10);

	if(isPair)
		SetStrategy(new PairStrategy(first, second, dealer_card));
	else if(isSoft)
		SetStrategy(new SoftStrategy(min, dealer_card));
	else
		SetStrategy(new HardStrategy(sum, dealer_card));

	return UseStrategy();
};

int main()
{
	int first, second, dealer_card;
	cout << "Dealer's card is [2, 3, 4, 5, 6, 7, 8, 9, 10, 11]: " << endl;
	cin >> dealer_card;
	cout << "Player's cards are [2, 3, 4, 5, 6, 7, 8, 9, 10, 11]: " << endl;
	cin >> first;
	cin >> second;
	cout << endl;
	Player John;
	cout << John.play_with_dealer(first, second, dealer_card) << endl;
}
