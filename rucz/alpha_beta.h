/**
 * \file alpha_beta.h
 * \brief interface for alpha / beta search
 * \ingroup alpha_beta
 * \author Peter Rucz rucz@hit.bme.hu 
 * \date 2015
 * \details
 * This implementation of the alpha / beta search interface is independent of 
 * the specific game.
 */

#ifndef ALPHA_BETA_H
#define ALPHA_BETA_H

/**
 * \brief maximal number of levels for the whole library
 * This macro definition  
 */
#define ALPHA_BETA_MAX_LEVELS (16)

/**
 * \defgroup alpha_beta_fun_types
 * \ingroup alpha_beta
 * @{
 */
/** \brief function type to get a size of an implementation depenedent type */
typedef size_t (*alpha_beta_get_size_fun_t)(void);

/**
 * \brief function type that implements a move on the board
 * \details
 * The first parameter of the function should be a pointer to a board entity
 * and the second parameter is a pointer to a move entity.
 */
typedef int (*alpha_beta_make_move_fun_t)(void *, void *);

/**
 * \brief function type that checks if the game has ended
 * \details
 * The function should return non-zero if the game is ended and zero otherwise.
 */
typedef int (*alpha_beta_is_game_ended_fun_t)(void *);

/**
 * \brief function type that returns the number of valid moves
 * \details
 * The first parameter of this function should be a pointer to a board entity,
 * and the second parameter should be a pointer in which the valid moves are
 * stored. The memory for the valid moves is allocated by the alpha beta engine
 * during initalization.
 */
typedef int (*alpha_beta_get_valid_moves_fun_t)(void *, void *);

/**
 * \brief function type to evaluate a position on the board
 * \details
 * The first parameter of this function should be a pointer to a board entity.
 * The second parameter is a pointer, which can contain any data type required 
 * by the AI for evaluating a position. This pointer is given to the function 
 * when calling the function ::alpha_beta_move.
 */
typedef double (*alpha_beta_eval_position_fun_t)(void *, void *);

/**
 * \brief function type that returns a move from the valid moves
 * \details
 * The first parameter of the function is the ordinal number of the requested 
 * move. The second parameter is a pointer to a valid moves entity. The third 
 * parameter is a pointer, where the selected move entity should be copied. 
 * The memory for this is allocated by the alpha beta framework.
 */
typedef int (*alpha_beta_get_move_fun_t)(int, void *, void *);
/** @} */

/**
 * \brief collection of implementation specific function pointers
 */
typedef struct {
	alpha_beta_get_size_fun_t get_move_size;
	alpha_beta_get_size_fun_t get_board_size;
	alpha_beta_make_move_fun_t make_move;
	alpha_beta_is_game_ended_fun_t is_game_ended;
	
	alpha_beta_get_size_fun_t get_valid_moves_size;
	alpha_beta_get_valid_moves_fun_t get_valid_moves;
	alpha_beta_eval_position_fun_t eval_position;
	alpha_beta_get_move_fun_t get_move;
} alpha_beta_funs_t;

/**
 * \brief data stored on each level of the alpha / beta algorithm
 * \details
 * The tree is built up in a depth-first-search manner.
 */
typedef struct {
	int depth;						/** \brief depth in the tree */
	int n_valid_moves;				/** \brief number of valid moves */
	int selected_move_idx;			/** \brief the index of selected moves */
	double selected_move_value;		/** \brief value of the selected move */
	int current_move_idx;			/** \brief index of the move currently being processed */
	int parent_move_idx;			/** \brief move index of parent */
	void * board;					/** \brief pointer to a board entity */
	void * valid_moves;				/** \brief pointer to valid moves */
} alpha_beta_level_data_t;

typedef struct {
	alpha_beta_funs_t funs;
	int n_levels;
	int enable_alpha_cuts;
	int enable_beta_cuts;
	
	size_t move_size;
	size_t board_size;
	size_t valid_moves_size;
	alpha_beta_level_data_t level_data[ALPHA_BETA_MAX_LEVELS];
	void * tmp_move;
	void * board_array;
	void * valid_moves_array;
} alpha_beta_data_t;

/** \brief statistics of alpha / beta examination */
typedef struct {
	unsigned int n_eval_pos;		/** \brief number of evaluated positions */
	unsigned int max_depth;			/** \brief maximal depth reached */
	unsigned int n_alpha_cuts;		/** \brief number of alpha cuts */
	unsigned int n_beta_cuts;		/** \brief number of beta cuts */
} alpha_beta_stats_t;

void alpha_beta_init(alpha_beta_data_t *abd);
void alpha_beta_finish(alpha_beta_data_t *abd);

/**
 * \brief move selection using the alpha / beta technique
 * \param[in] abd pointer to an alpha / beta data structure
 * \param[in] brd pointer to a board entity
 * \param[in] aid pointer to AI data
 * \param[out] mv pointer to the resulting move
 * \param[out] st statistics of running the alpha / beta algorithm
 * \return top-level value
 */
double alpha_beta_move(
	alpha_beta_data_t *abd, 
	void *brd, 
	void* aid, 
	void *mv, 
	alpha_beta_stats_t *st
);

#endif /* ALPHA_BETA_H */
