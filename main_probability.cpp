#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::endl;
using std::vector;

int main()
{
	srand(time(NULL));

	/*! 13 card for each suit.
	 ** Ace is 11, J,Q and K are 10 */
	vector<int> deck = {
		2,3,4,5,6,7,8,9,10,10,10,10,11,
		2,3,4,5,6,7,8,9,10,10,10,10,11,
		2,3,4,5,6,7,8,9,10,10,10,10,11,
		2,3,4,5,6,7,8,9,10,10,10,10,11
	};

	/*! Take one random integer from deck container */
	auto randomCard = [&]() -> int {
		return deck[rand() % deck.size()];
	};

	/*! Return true if dealer has [18;21] sum, otherwise false */
	/** sum - the first (faced up) dealer card */
	auto dealerWins = [&](int sum = 0) -> bool {
		while(sum <= 17) {
			int card = randomCard();
			/** dealer take Ace card as 1 (not as 11)
			 ** but only if otherwise he fails */
			if(card == 11 && sum + card > 21)
				card = 1;
			sum += card;
			if(sum > 21)
				return false;
		} 
		return true;
	};

	/*! Return the probability of the dealer to win when
	 ** he starts with "faced_up" card */
	/** accurance - the quantity of experiments */
	auto dealerStartsWith = [&](int faced_up, int accurance = 1000) -> double {
		int wins = 0;
		int fails = 0;
		for(int i = 0; i < accurance; i++) {
			(dealerWins(faced_up))? wins++ : fails++;
		}
		return (double) wins / (wins + fails) * 100;
	};

	for(int i = 2; i <= 11; i++)
		cout << "Faced up card is " << i << " : " << dealerStartsWith(i) << '%' << endl;
}