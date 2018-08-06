/* -----------------------------------------------------------------------
 * Random test for case great_hall in cardEffect()
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <time.h>

void assertTrue(int a, int b, int flag) {
  if (a == b) {
    printf("PASS");
  } else {
    printf("FAIL");
    flag = 1;
  }
  printf(" -- expected %d, got %d\n", a, b);
}

int main() {
  int numPlayers;
  int k[10] = {adventurer, council_room, feast, gardens, mine, 
               remodel, smithy, village, baron, great_hall};
  srand(time(0));
  int randomSeed;
  struct gameState *testGame = 0;
  int n;
  int i;
  int j;
  int r;
  int value1;
  int value2;
  int choice1 = 0;
  int choice2 = 0;
  int choice3 = 0;
  int handPos = 0;
  int *bonus = 0;
  int flag = 0;

  printf("Random testing for greatHallEffect():\n\n");

  for (n = 0; n < 2000; n++) {
    randomSeed = rand();

    printf("Randomizing number of players\n\n");
    numPlayers = rand() % 30 + 2;

    r = initializeGame(numPlayers, k, randomSeed, testGame);

    if (r == -1) {
      printf("Game initialization failed -- unable to test greatHallEffect()!\n");
      return 1;
    }

    printf("Randomizing deck counts, hand counts, and discard counts\n\n");
    for (i = 0; i < numPlayers; i++) {
      testGame->deckCount[i] = rand() % 200;
      testGame->handCount[i] = rand() % 200;
      testGame->discardCount[i] = rand() % 200;
      printf("Starting deck count for player %d: %d\n", i+1, testGame->deckCount[i]);
      printf("Starting hand count for player %d: %d\n", i+1, testGame->handCount[i]);
      printf("Starting discard count for player %d: %d\n", i+1, testGame->discardCount[i]);
    }
    
    printf("\nEach player will play a great hall.\nTesting one at a time:\n");

    for (i = 0; i < numPlayers; i++) {
      testGame->whoseTurn = i;
      printf("Randomizing number of actions and hand position\n");
      testGame->numActions = rand() % 30;
      handPos = rand() % testGame->handCount[i];
      value1 = testGame->numActions + 1;
      value2 = testGame->handCount[i];
      j = cardEffect(adventurer, choice1, choice2, choice3, testGame, handPos, bonus);
      if (j != 0) {
        flag = 1;
        printf("ERROR -- greatHallEffect() did not return correctly\n");
      }
      printf("Player %d now has %d actions\n", i+1, testGame->numActions);
      assertTrue(value1, testGame->numActions, flag);
      printf("Player %d now has %d cards in hand\n", i+1, testGame->handCount[i]);
      assertTrue(value2, testGame->handCount[i], flag);
    }
  }

  if (flag == 0) {
    printf("All tests passed!\n\n");
  } else {
    printf("ERROR -- TESTS FAILED!\n\n");
  }

  return 0;
}
