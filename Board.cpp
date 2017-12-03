//******************************************************************************
// Board.cpp
// Revision History:
//
// Date           Author               Description
// 11/28/2017     Jason Chen           Added class
//******************************************************************************

#include <iostream>
#include <sstream>
#include "Deck.h"
#include "Hand.h"
#include "Card.h"
#include "Board.h"
#include "Player.h"
#include "ConsoleUI.h"
#include "helper.h"
using namespace std;


//constructor
//initialize the board for a new game
Board::Board(Player* hum, Player* AI, int smallBlindPlayer)
{
	human = hum;
	this->AI=AI;
	setCommunity();
	help=new helper();
	this->smallBlindPlayer=smallBlindPlayer;
}

void Board::setBlind(int bld)
{
	smallBlind = bld;
}

void Board::setCommunity()
{
	for(int i = 0; i < 5; i++)
	{
		community[i] = dek.draw();
	}
	//set player cards
	human->addOne(dek.draw());
	human->addTwo(dek.draw());
	AI->addOne(dek.draw());
	AI->addTwo(dek.draw());
}

void Board::printBoard()
{
	cout << "The pot is: " << pot << endl;
	cout << "Your stack size is: " << human->getTotalChips() << endl;
	cout << "AI's stack size is: " << AI->getTotalChips() << endl << endl;
}

bool Board::run()
{
	string inputTemp;
	ConsoleUI* ui = new ConsoleUI();
	if(human->getTempPool() < AI->getTempPool())
	{
		inputTemp=ui->input("Fold (1), Call (2), or Raise (3)\n");
		while (!help->isInt(inputTemp)||stoi(inputTemp)<1||stoi(inputTemp)>3)
		{
			ui->output("Input must be an integer.");
			inputTemp=ui->input("Fold (1), Check (2), or Raise (3)");
		}
	}
	else
	{
		inputTemp=ui->input("Fold (1), Check (2), or Raise (3)\n");
		while (!help->isInt(inputTemp)||stoi(inputTemp)<1||stoi(inputTemp)>3)
		{
			ui->output("Input must be an integer.");
			inputTemp=ui->input("Fold (1), Check (2), or Raise (3)");
		}
	}
	if(inputTemp == "1")
	{
		return true;
	}
	else if(inputTemp == "2")
	{
		human->call(AI);
		pot=human->getTempPool()+AI->getTempPool();
		printBoard();
		return false;
	}
	else
	{
		int prev = AI->getPrevBet();
		string r = ui->input("How much do you want to raise by?");
		while (!help->isInt(r)|| stoi(r) < prev*2 || stoi(r) > human->getTotalChips())
		{
			ui->output("Input must be an integer and at least double the previous bet.");
			r=ui->input("How much do you want to raise by?");
		}
		human->raise(stoi(r));
		pot=human->getTempPool()+AI->getTempPool();
		human->setPrevBet(stoi(r));
		printBoard();
		return false;
	}

}

bool Board::runAI(){
	ConsoleUI* ui = new ConsoleUI();
	ui->output("AI's turn: ");
	AI->call(human); //for now
	pot=AI->getTempPool()+human->getTempPool();
	printBoard();
	return false;
}
bool Board::preflop()
{
	printBoard();
	ConsoleUI* ui=new ConsoleUI();
	string inputTemp;
	//print user's hand
	ui->output("Your hand: ");
	(human->getHandOne()).printCard();
	(human->getHandTwo()).printCard();
	//force user to put in blind
	pot=smallBlind*3;
	if (smallBlindPlayer==1){
		AI->raise(smallBlind);
		human->raise(smallBlind*2);
	}
	else{
		human->raise(smallBlind);
		AI->raise(smallBlind*2);
	}
	cout<<"AI total chips: "<<AI->getTotalChips()<<endl; //980
	cout<<"Human total chips: "<<human->getTotalChips()<<endl;  //990
	cout<<"small blind player : "<<smallBlindPlayer<<endl;
	while(human->getTempPool()!=AI->getTempPool()) //player facing a bet
	{
		if (smallBlindPlayer==1){
			//AI goes first
			this->runAI();
		}
		else if (smallBlindPlayer==0){
			//player goes first
			bool flag=this->run();
			if (flag==true){
				return true;
			}
		}
	}
	printBoard();
	return false;
}

bool Board::flop()
{
	printBoard();
	ConsoleUI* ui=new ConsoleUI();
	cout << "The flop is " << endl;
	community[0].printCard();
	community[1].printCard();
	community[2].printCard();
	//print user's hand
	ui->output("Your hand: ");
	(human->getHandOne()).printCard();
	(human->getHandTwo()).printCard();

	cout<<"AI total chips: "<<AI->getTotalChips()<<endl; //980
	cout<<"Human total chips: "<<human->getTotalChips()<<endl;  //990
	cout<<"small blind player : "<<smallBlindPlayer<<endl;
	do
	{
		if (smallBlindPlayer==1){
			//AI goes first
			this->runAI();
			bool flag=this->run();
			if (flag==true){
				return true;
			}
		}
		else if (smallBlindPlayer==0){
			//player goes first
			bool flag=this->run();
			if (flag==true){
				return true;
			}
			this->runAI();
		}
	}
	while(human->getTempPool()!=AI->getTempPool()); //player facing a bet
	printBoard();
	return false;
}

bool Board::turn()
{
	printBoard();
	cout << "The turn is " << endl;
	community[0].printCard();
	community[1].printCard();
	community[2].printCard();
	community[3].printCard();

	ConsoleUI* ui=new ConsoleUI();
	//print user's hand
	ui->output("Your hand: ");
	(human->getHandOne()).printCard();
	(human->getHandTwo()).printCard();

	cout<<"AI total chips: "<<AI->getTotalChips()<<endl; //980
	cout<<"Human total chips: "<<human->getTotalChips()<<endl;  //990
	cout<<"small blind player : "<<smallBlindPlayer<<endl;
	do
	{
		if (smallBlindPlayer==1){
			//AI goes first
			this->runAI();
			bool flag=this->run();
			if (flag==true){
				return true;
			}
		}
		else if (smallBlindPlayer==0){
			//player goes first
			bool flag=this->run();
			if (flag==true){
				return true;
			}
			this->runAI();
		}
	}
	while(human->getTempPool()!=AI->getTempPool()); //player facing a bet
	printBoard();
	return false;
}

bool Board::river()
{
	printBoard();
	cout << "The flop is " << endl;
	community[0].printCard();
	community[1].printCard();
	community[2].printCard();
	community[3].printCard();
	community[4].printCard();
	ConsoleUI* ui=new ConsoleUI();
	//print user's hand
	ui->output("Your hand: ");
	(human->getHandOne()).printCard();
	(human->getHandTwo()).printCard();

	cout<<"AI total chips: "<<AI->getTotalChips()<<endl; //980
	cout<<"Human total chips: "<<human->getTotalChips()<<endl;  //990
	cout<<"small blind player : "<<smallBlindPlayer<<endl;
	do
	{
		if (smallBlindPlayer==1){
			//AI goes first
			this->runAI();
			bool flag=this->run();
			if (flag==true){
				return true;
			}
		}
		else if (smallBlindPlayer==0){
			//player goes first
			bool flag=this->run();
			if (flag==true){
				return true;
			}
			this->runAI();
		}
	}
	while(human->getTempPool()!=AI->getTempPool()); //player facing a bet
	printBoard();

	//after everything is done
	Hand* humanBest=help->bestHand((human->getHandOne()),(human->getHandTwo()),
					community[0],community[1],community[2],community[3],community[4]);
	Hand* AIBest=help->bestHand((AI->getHandOne()),(AI->getHandTwo()),
			community[0],community[1],community[2],community[3],community[4]);
	int result=help->compareHands(humanBest,AIBest);
	ui->output("Community Cards: ");
	for (int i=0;i<5;i++){
		community[i].printCard();
	}
	ui->output("AI's cards: ");
	AI->getHandOne().printCard();
	AI->getHandTwo().printCard();
	ui->output("Your cards: ");
	human->getHandOne().printCard();
	human->getHandTwo().printCard();

	ui->output("Your best hand: ");
	humanBest->printHand();
	ui->output("");
	ui->output("AI's best hand: ");
	AIBest->printHand();
	if (result==1){
		ui->output("You won.");
		human->setTotalChips(human->getTotalChips()+pot);
	}
	else if (result==0){
		ui->output("AI won");
		AI->setTotalChips(AI->getTotalChips()+pot);
	}
	else{
		ui->output("Tie");
		human->setTotalChips(human->getTotalChips()+pot/2);
		AI->setTotalChips(AI->getTotalChips()+pot/2);
	}
	//cout<<"Result: "<<result<<endl;
	human->resetTempPool();
	AI->resetTempPool();
	pot=0;

	return false;
}

void Board::clearBoard()
{
	dek.shuffle();
	setCommunity();
	if (smallBlindPlayer==0){
		smallBlindPlayer=1;
	}
	else{
		smallBlindPlayer=0;
	}
}
