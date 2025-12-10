#pragma once

/**
 * @brief Suits enumeration, aligned with CardSuitType in the requirement doc.
 */
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,     ///< Clubs
    CST_DIAMONDS,  ///< Diamonds
    CST_HEARTS,    ///< Hearts
    CST_SPADES,    ///< Spades
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * @brief Rank enumeration, aligned with CardFaceType in the requirement doc.
 */
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};


