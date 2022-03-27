#include<Bangtal.h>

#include<stdlib.h>
#include<time.h>

SceneID scene;
ObjectID game_board[9], init_board[9];
ObjectID start;

const char* board_image[9] = {
	"hmbb-1.png",
	"hmbb-2.png",
	"hmbb-3.png",
	"hmbb-4.png",
	"hmbb-5.png",
	"hmbb-6.png",
	"hmbb-7.png",
	"hmbb-8.png",
	"hmbb-9.png",
};
const int board_x[9] = { 0,200,400,0,200,400,0,200,400 };
const int board_y[9] = { 400,400,400,200,200,200,0,0,0 };

int blank;

TimerID timer;
int mixCount;

int board_index(ObjectID object) {
	for (int i = 0; i < 9; i++) {
		if (game_board[i] == object) return i;
	}
	return -1;
}

void board_move(int index) {
	ObjectID t = game_board[blank];
	game_board[blank] = game_board[index];
	game_board[index] = t;

	locateObject(game_board[blank], scene, board_x[blank], board_y[blank]);
	locateObject(game_board[index], scene, board_x[index], board_y[index]);

	blank = index;
}

bool movable(int index) {
	if (blank < 0 or blank > 8)return false;
	if (blank % 3 != 0 and blank - 1 == index) return true;
	if (blank % 3 != 2 and blank + 1 == index) return true;
	if (blank / 3 != 0 and blank - 3 == index) return true;
	if (blank / 3 != 2 and blank + 3 == index) return true;

	return false;
}

void board_mix() {
	int index;

	do {
		switch (rand() % 3) {
		case 0: index = blank - 1; break;
		case 1: index = blank + 1; break;
		case 2: index = blank - 3; break;
		case 3: index = blank + 3; break;
		}
	} while (!movable(index));
	board_move(index);
}

bool completed() {
	for (int i = 0; i < 9; i++) {
		if (game_board[i] != init_board[i])return false;
	}

	return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		mixCount = 10;

		setTimer(timer, 1.f);
		startTimer(timer);
	}
	else {
		int index = board_index(object);
		if (movable(index)) {
			board_move(index);

			if (completed()) {
				showMessage("Completed!!!");
			}
		}
	}
}

void timerCallback(TimerID timer) {
	mixCount--;

	if (mixCount > 0) {
		board_mix();

		setTimer(timer, 1.f);
		startTimer(timer);
	}
}

int main() {
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene = createScene("hmbb", "beijing.png");

	for (int i = 0; i < 9; i++) {
		game_board[i] = createObject(board_image[i]);
		init_board[i] = game_board[i];

		locateObject(game_board[i], scene, board_x[i], board_y[i]);
		showObject(game_board[i]);
	}

	blank = 8;


	start = createObject("start.png");
	locateObject(start, scene, 250, 200);
	showObject(start);

	timer = createTimer(1.f);

	startGame(scene);

	return 0;

}