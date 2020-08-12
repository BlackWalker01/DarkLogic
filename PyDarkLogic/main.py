from MainDarkLogic.logicgame import LogicGame
import Test.test
import argparse


parser = argparse.ArgumentParser()
parser.add_argument("--type", default="game", type=str, help="type of run, either game or test")
parser.add_argument("--testName", default="", required=False)
parser.add_argument("--auto", default=False, type=bool, required=False)
args = parser.parse_args()

if args.type == "game":
    if args.auto:
        game = LogicGame(True)
    else:
        game = LogicGame()
    game.start()
elif args.type == "test":
    if args.testName != "":
        exit(Test.test.run(args.testName))
    else:
        print("[ERROR] TestName is missing!")
        exit(1)
