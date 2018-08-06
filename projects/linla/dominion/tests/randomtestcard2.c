/* -----------------------------------------------------------------------
 * Random test for baronEffect()
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
  int value3;
  int value4;
  int choice1;
  int flag = 0;

  printf("Random testing for baronEffect():\n\n");

  for (n = 0; n < 2000; n++) {
    randomSeed = rand();

    printf("Randomizing number of players\n\n");
    numPlayers = rand() % 30 + 2;

    r = initializeGame(numPlayers, k, randomSeed, testGame);

    if (r == -1) {
      printf("Game initialization failed -- unable to test baronEffect()!\n");
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
    
    printf("\nEach player will play a baron.\nTesting one at a time:\n");

    for (i = 0; i < numPlayers; i++) {
      testGame->whoseTurn = i;
      printf("Randomizing number of buys, coins, and player choice\n");
      testGame->numActions = rand() % 30;
      testGame->coins = rand() % 100;
      choice1 = rand() % 1000;
      if (choice1 > 350) {
        choice1 = 1;
      } else {
        choice1 = -1;
      }
      printf("Randomizing supply count of estates\n");
      testGame->supplyCount[estate] = rand() % 30;
      value1 = testGame->numBuys + 1;
      value2 = testGame->coins + 4;
      value3 = testGame->discardCount[i] + 1;
      value4 = testGame->handCount[i] - 1;
      j = baronEffect(testGame, choice1, i);
      if (j != 0) {
        flag = 1;
        printf("ERROR -- baronEffect() did not return correctly\n");
      }
      printf("Player %d now has %d buys\n\n", i+1, testGame->numBuys);
      assertTrue(value1, testGame->numBuys, flag);
      if (testGame->coins == value2) {
        printf("Player %d discarded an estate and now has %d coins\n", i+1, testGame->coins);
        assertTrue(value2, testGame->coins, flag);
        printf("Player %d now has %d cards in discard\n", i+1, testGame->discardCount[i]);
        assertTrue(value3, testGame->discardCount[i], flag);
        printf("Player %d now has %d cards in hand\n", i+1, testGame->handCount[i]);
        assertTrue(value4, testGame->handCount[i], flag);
      } else if (testGame->discardCount[i] == value3) {
        printf("Player %d gained an estate and now has %d cards in discard\n", i+1, testGame->discardCount[i]);
        assertTrue(value3, testGame->discardCount[i], flag);
        printf("Player %d still has %d coins\n", i+1, testGame->coins);
        assertTrue(value2-4, testGame->coins, flag);
        printf("Player %d still has %d cards in hand\n", i+1, testGame->handCount[i]);
        assertTrue(value4+1, testGame->handCount[i], flag);
      } else if (testGame->coins == value2-4) {
        printf("No more estates left");
        assertTrue(0, testGame->supplyCount[estate], flag);
        printf("Player %d still has %d coins\n", i+1, testGame->coins);
        assertTrue(value2-4, testGame->coins, flag);
        printf("Player %d still has %d cards in discard\n", i+1, testGame->discardCount[i]);
        assertTrue(value3-1, testGame->discardCount[i], flag);
        printf("Player %d still has %d cards in hand\n", i+1, testGame->handCount[i]);
        assertTrue(value4+1, testGame->handCount[i], flag);
      }
    }
  }

  if (flag == 0) {
    printf("All tests passed!\n\n");
  } else {
    printf("ERROR -- TESTS FAILED!\n\n");
  }

  return 0;
}
