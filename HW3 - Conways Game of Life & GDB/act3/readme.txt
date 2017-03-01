Douglas Gliner

Launch Params
./good-life BOARD_WIDTH BOARD_HEIGHT<optional>
- Providing only BOARD_WIDTH will result in the same value being used for
  BOARD_HEIGHT.
- Providing no params will set the board to default width/height of 20.

Random Birth
- There is a 5% chance that a single cell organism will spawn in a random
  location that isnt occupied. If the space is occupied, nothing happens!

Special Organisms
- All three specified organisms are implemented and it is extremely easy
  to add your own! Simply construct a char matrix of the size of your 
  special organism, construct it like so:
  
    static const char blinkerTemplate[3][3] = {
        { ' ', '*', ' ' },
        { ' ', '*', ' ' },
        { ' ', '*', ' ' }
    };
    
  Then call this neat function with specified params
  
  /// Spawns a special organism starting at corner of given location.
  /// NOTE: Special organism will overwrite any cells at given location!!
  ///
  /// @param row  row to spawn special organism
  /// @param col  column to spawn special organism
  /// @param width  width of provided matrix
  /// @param height  height of provided matrix
  /// @param life  matrix of organisms where each organism is an asterisk
  /// @param specWidth  width of provided special organism template
  /// @param specHeight  height of provided special organism template
  /// @param specialTemplate  matrix of characters for a special organism
  void spawnSpecial(const int row, const int col,
	const int width, const int height, char life[height][width],
	const int specWidth, const int specHeight,
	const char specTemplate[specHeight][specWidth])
    
  Just like that, you've added your own organism.
  
  AWW ISNT HE CUTE . 3.