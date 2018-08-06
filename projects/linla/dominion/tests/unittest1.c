/* -----------------------------------------------------------------------
 * Unit test for drawCard()
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
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
  int numPlayers = 2;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  srand(time(0));
  int randomSeed = rand();
  struct gameState testGame;
  int i;
  int j;
  int value1;
  int value2;
  int flag = 0;

  printf("Testing drawCard():\n\n");

  int r = initializeGame(numPlayers, k, randomSeed, &testGame);

  if (r == -1) {
    printf("Game initialization failed -- unable to test drawCard()!\n");
    return 1;
  }

  value1 = testGame.deckCount[0];
  value2 = testGame.deckCount[1];
  printf("Starting deck count for player %d: %d\n", 1, value1);
  printf("Starting deck count for player %d: %d\n\n", 2, value2);
  printf("Each player will draw 5 cards.\nTesting one at a time:\n");

  for (i = 0; i < numPlayers; i++) {
    for (j = 0; j < 5; j++) {
      drawCard(i, &testGame);
      printf("Player %d now has %d cards in hand\n", i+1, testGame.handCount[i]);
      assertTrue(j+1, testGame.handCount[i], flag);
    }
  }

  printf("Player %d now has %d cards in deck\n", 1, testGame.deckCount[0]);
  assertTrue(value1 - 5, testGame.deckCount[0], flag);
  printf("Player %d now has %d cards in deck\n\n", 2, testGame.deckCount[1]);
  assertTrue(value2 - 5, testGame.deckCount[1], flag);

  printf("Discarding all cards for both players\n");
  testGame.discardCount[0] = testGame.deckCount[0];
  testGame.discardCount[1] = testGame.deckCount[1];
  testGame.deckCount[0] = 0;
  testGame.deckCount[1] = 0;

  printf("Player %d now has %d cards discarded\n", 1, testGame.discardCount[0]);
  printf("and %d cards in deck\n", 1, testGame.deckCount[0]);
  printf("Player %d now has %d cards discarded\n", 2, testGame.discardCount[1]);
  printf("and %d cards in deck\n", 1, testGame.deckCount[1]);

  printf("Drawing one card each...\n");
  for (i = 0; i < numPlayers; i++) {
    drawCard(i, &testGame);
    printf("Player %d now has %d cards in hand\n", i+1, testGame.handCount[i]);
    assertTrue(6, testGame.handCount[i], flag);
    printf("Player %d should have no more cards discarded\n", i+1);
    assertTrue(0, testGame.discardCount[i], flag);
  }

  if (flag == 0) {
    printf("All tests passed!\n\n");
  } else {
    printf("ERROR -- TESTS FAILED!\n\n");
  }

  return 0;
}
