////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEngRenderInfo.h
//
//  Information needed to render 
//  a single texture using the
//  SDL_RenderCopy or
//  SDL_RenderCopyEx command
//
//  This file has no corresponding .cpp file
//
//  Copyright (c) Muckytaters 2023
//
//  This program is free software: you can
//  redistribute it and/or modify it under 
//  the terms of the GNU General Public License
//  as published by the Free Software Foundation,
//  either version 3 of the License, or (at your
//  option) any later version.
//
//  This program is distributed in the hope it
//  will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU
//  General Public License along with this
//  program. If not, see http://www.gnu.org/license
////////////////////////////////////////////

#ifndef MCK_GAME_ENG_RI_H
#define MCK_GAME_ENG_RI_H

#include "GameEngRenderBase.h"

namespace MCK
{

// Forward declaration
class GameEngRenderBlock;
class SpriteAnimTime;
class SpriteAnimDist;

class GameEngRenderInfo : public GameEngRenderBase
{
    //! Friendship so GameEng can access protected/private members
    friend class GameEng;
    friend class SpriteAnimTime;
    friend class SpriteAnimDist;

    public:

        //!Non-SDL wrapper for SDL_Rect
        struct Rect
        {
            //! Friendship to access protected/private members
            friend class GameEng;
            friend class GameEngRenderInfo;
           
            //! Get left position of rectangle
            int get_x( void ) const noexcept { return this->r.x; }
            
            //! Get top position of rectangle
            int get_y( void ) const noexcept { return this->r.y; }

            //! Get width of rectangle
            int get_w( void ) const noexcept { return this->r.w; }

            //! Get height of rectangle
            int get_h( void ) const noexcept { return this->r.h; }

            //! Get position of right edge
            int get_right( void ) const noexcept
            {
                return this->r.x + this->r.w;
            }
    
            //! Get position of bottom edge
            int get_bottom( void ) const noexcept
            {
                return this->r.y + this->r.h;
            }
    
            //! Set left position of rectangle
            void set_x( int val ) noexcept { this->r.x = val; }
            
            //! Set top position of rectangle
            void set_y( int val ) noexcept { this->r.y = val; }

            //! Set width of rectangle
            void set_w( int val ) noexcept { this->r.w = val; }

            //! Set height of rectangle
            void set_h( int val ) noexcept { this->r.h = val; }

            void set_pos( int x, int y ) noexcept
            {
                this->r.x = x;
                this->r.y = y;
            }

            void set_all( int x, int y, int w, int h ) noexcept
            {
                SDL_Rect &rct = this->r;
                rct.x = x;
                rct.y = y;
                rct.w = w;
                rct.h = h;
            }

            void adj_pos( int dx, int dy ) noexcept
            {
                this->r.x += dx;
                this->r.y += dy;
            }

            //! Default constructor
            Rect( void )
            {
                this->r.x = 0;
                this->r.y = 0;
                this->r.w = 0;
                this->r.h = 0;
            }

            //! Constructor
            /* @param _x: Left position of rectangle
             * @param _y: Left position of rectangle
             * @param _w: Width of rectangle
             * @param _h: Height position of rectangle
             */
            Rect( int _x, int _y, int _w, int _h )
            {
                this->r.x = _x;
                this->r.y = _y;
                this->r.w = _w;
                this->r.h = _h;
            }
            
            private:

                //! SDL rectangle is private so direct use is only by GameEng
                SDL_Rect r;
        };

        //! Texture ID
        /* Added for convenience, as 'tex' is private.
         * Should be the correct ID of 'tex',
         * but this is not enforced */
        MCK_TEX_ID_TYPE tex_id; 

        //! Destination rectangle of texture in game window
        /*! This is measured in *screen* pixels,
         *  relative to top-left of game window.
         */
        MCK::GameEngRenderInfo::Rect dest_rect;

        //! Set x position
        virtual void set_x( int x ) noexcept
        {
            this->dest_rect.set_x( x );
        }

        //! Set y position
        virtual void set_y( int y ) noexcept
        {
            this->dest_rect.set_y( y );
        }

        //! Set position
        virtual void set_pos( int x, int y ) noexcept
        {
            this->dest_rect.set_pos( x, y );
        }

        //! Get x position
        virtual int get_x( void ) const noexcept
        {
            return this->dest_rect.get_x();
        }

        //! Get y position
        virtual int get_y( void ) const noexcept
        {
            return this->dest_rect.get_y();
        }

        //! Get width
        virtual int get_width( void ) const noexcept
        {
            return this->dest_rect.get_w();
        }

        //! Get height position
        virtual int get_height( void ) const noexcept
        {
            return this->dest_rect.get_h();
        }

        //! Get position of bottom edge
        virtual int get_bottom( void ) const noexcept
        {
            return this->dest_rect.get_bottom();
        }
    
        //! Get position of right-hand edge
        virtual int get_right( void ) const noexcept
        {
            return this->dest_rect.get_right();
        }

        //! Adjust position
        virtual void adj_pos( int dx, int dy ) noexcept
        {
            this->dest_rect.adj_pos( dx, dy );
        }

        //! Flag to indicate whether clipping rectangle used
        bool clip;

        //! Clipping rectangle for source texture
        /*! This is measured in source texture's 
         * own pixel coords.
         */
        MCK::GameEngRenderInfo::Rect clip_rect;

        //! Get rotation, as multiple of 90 degrees
        int get_rotation( void ) const noexcept
        {
            return ( this->flags & MCK::GameEngRenderInfo::ROTATION_MASK ) 
                        >> ROTATION_RSHIFT;
        }

        //! Get flip x
        bool get_flip_x( void ) const noexcept
        {
            return ( this->flags & MCK::GameEngRenderInfo::FLIP_X_MASK ) > 0;
        }

        //! Get flip y
        bool get_flip_y( void ) const noexcept
        {
            return ( this->flags & MCK::GameEngRenderInfo::FLIP_Y_MASK ) > 0;
        }

        //! Default constructor
        GameEngRenderInfo( uint32_t z = MCK::DEFAULT_Z_VALUE )
            : GameEngRenderBase( z )
        {
            this->clip = false;
            this->flags = 0x00;
            this->tex_id = 0;
            this->tex = NULL;
            
            this->type = MCK::RenderInstanceType::INFO;
        }

        //! Render
        virtual void render( SDL_Renderer* renderer ) const
        {
            // Let calling method catch any exceptions

            // If no flags, use simpler command
            if( this->flags == 0 )
            {
                SDL_RenderCopy(
                    renderer,
                    this->tex,
                    this->clip ? &this->clip_rect.r : NULL,
                    &this->dest_rect.r
                );
            }
            else
            {
                // Get rotation angle (in degrees)
                const double ANGLE
                    = ( 
                        ( this->flags & MCK::GameEngRenderInfo::ROTATION_MASK )
                            >> MCK::GameEngRenderInfo::ROTATION_RSHIFT
                    ) * 90.0f;
            
                // Get flip
                // TODO: Make this more efficient
                SDL_RendererFlip flip = SDL_FLIP_NONE;
                if( this->get_flip_x() )
                {
                    flip = SDL_RendererFlip( flip | SDL_FLIP_HORIZONTAL );
                }
                if( this->get_flip_y() )
                {
                    flip = SDL_RendererFlip( flip | SDL_FLIP_VERTICAL );
                }
                
                SDL_RenderCopyEx(
                    renderer,
                    this->tex,  // Convert from shared_ptr
                    this->clip ? &this->clip_rect.r : NULL,
                    &this->dest_rect.r,
                    ANGLE,
                    NULL,  // Rotate about centre
                    flip
                );
            }
        } 
            
        //! Calculate flag byte
        /*! Use to calculate flag values for sprite frames.*/
        static uint8_t calc_flags(
            int rotation,
            bool flip_x,
            bool flip_y
        )
        {
            return ( ( rotation % 4 ) << ROTATION_RSHIFT )
                   | ( flip_x ? FLIP_X_MASK : 0 )
                   | ( flip_y ? FLIP_Y_MASK : 0 );
        }
        

    protected:

        //! Set flags for rotation and x/y flip (always use 'calc_flags' method to obtain a valid value)
        void set_flags( uint8_t _flags ) noexcept
        {
            this->flags = _flags;
        }

        // Disabled to prevent copying,
        // as this would cause chaos
        GameEngRenderInfo(GameEngRenderInfo const&) = delete;
        void operator=(GameEngRenderInfo const&)  = delete;

        const static uint8_t ROTATION_MASK = 0x03;
        const static uint8_t ROTATION_RSHIFT = 0;
        const static uint8_t FLIP_X_MASK = 0x04;
        const static uint8_t FLIP_Y_MASK = 0x08;

        //! Pointer to source texture
        /*! Game engine accesses this through friend status*/
        SDL_Texture* tex;

        //! Flags to indicate rotation and flipping
        /*! See flag masks */
        uint8_t flags;
        
        // Dummy method to this class non-abstract
        void dummy( void ) {}
};

}  // End of namespace MCK

#endif
