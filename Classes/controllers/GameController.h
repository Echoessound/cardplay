#pragma once

#include "cocos2d.h"

class GameModel;
class GameView;
class UndoManager;
class GameModelFromLevelGenerator;

/**
 * @brief Main gameplay controller that wires models and views together.
 *
 * Responsibilities:
 * - Build a GameModel from the requested level configuration.
 * - Handle user input events forwarded from the views.
 * - Drive UndoManager to persist and restore operations.
 * - Ask GameView to play the required animations.
 */
class GameController
{
public:
    GameController();
    ~GameController();

    /**
     * @brief Start a specific level.
     * @param levelId Level identifier.
     * @param parent  Parent node that hosts GameView (Scene in most cases).
     * @return Whether the level boot succeeded.
     */
    bool startGame(int levelId, cocos2d::Node* parent);

    /**
     * @brief Entry point for playfield card clicks.
     * @param cardId Logical card identifier that was clicked.
     * @return True if the interaction is valid and processed.
     */
    bool handlePlayfieldCardClick(int cardId);

    /**
     * @brief Entry point for stack card clicks (tray/backup pile).
     *        Used by the "stack swap" feature.
     * @param cardId Logical card identifier that was clicked.
     * @return True if the interaction is valid and processed.
     */
    bool handleStackCardClick(int cardId);

    /**
     * @brief Handle the undo button.
     *        Attempts to pop a record from UndoManager and roll it back.
     */
    void handleUndo();

    /** Get a read-only pointer to the GameModel for inspection. */
    const GameModel* getGameModel() const
    {
        return _gameModel;
    }

private:
    GameModel* _gameModel;                 ///< Runtime game data
    GameView* _gameView;                   ///< Game view root node
    UndoManager* _undoManager;             ///< Manages undo records

    GameModelFromLevelGenerator* _generator; ///< Service that converts LevelConfig into GameModel

    bool initGameModel(int levelId);
    bool initGameView(cocos2d::Node* parent);
    void bindViewCallbacks();
};



