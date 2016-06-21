#import "GLEssentialsGLView.h"
#import "SbMain.h"
#import "BtTimeGLES.h"
#import "BtTime.h"
#import "RsImpl.h"
#import "ApConfig.h"
#import "SdSoundWinGL.h"
#import "UiKeyboard.h"
#import "UiInputWinGL.h"
#import "psmove.h"
#import "ShIMU.h"
#import "MtVector3.h"
#import "HlKeyboard.h"
#import "glutil.h"
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/AudioServices.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/AudioServices.h>
#import <AVFoundation/AVFoundation.h>
#include <iostream>
#include <sys/time.h>                // for gettimeofday()
#include "stdio.h"                   // for gettimeofday()
#import <queue>

#define SUPPORT_RETINA_RESOLUTION 1
//#define UseMadgwick

#ifdef UseMadgwick
#import "MadgwickAHRS.h"
#endif

const BtU32 Identity1   = 1;
const BtU32 Identity2   = 2;

//BtU32 PlayConfig = Identity2;
BtU32 PlayConfig = Identity1;

AVAudioPlayer* audioPlayer;

@interface GLEssentialsGLView (PrivateMethods)
- (void) initGL;

@end

@implementation GLEssentialsGLView

SbMain myProject;

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    // There is no autorelease pool when this method is called
    // because it will be called from a background thread.
    // It's important to create one or app can leak objects.
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    [self drawView];
    
    [pool release];
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink,
                                      const CVTimeStamp* now,
                                      const CVTimeStamp* outputTime,
                                      CVOptionFlags flagsIn,
                                      CVOptionFlags* flagsOut,
                                      void* displayLinkContext)
{
    CVReturn result = [(GLEssentialsGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (void) awakeFromNib
{
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        // Must specify the 3.2 Core Profile to use OpenGL 3.2
#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
#endif
        0
    };
    
    NSOpenGLPixelFormat *pf = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attrs] autorelease];
    
    if (!pf)
    {
        NSLog(@"No OpenGL pixel format");
    }
	   
    NSOpenGLContext* context = [[[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil] autorelease];
    
#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 && defined(DEBUG)
    // When we're using a CoreProfile context, crash if we call a legacy OpenGL function
    // This will make it much more obvious where and when such a function call is made so
    // that we can remove such calls.
    // Without this we'd simply get GL_INVALID_OPERATION error for calling legacy functions
    // but it would be more difficult to see where that function was called.
    CGLEnable([context CGLContextObj], kCGLCECrashOnRemovedFunctions);
#endif
    
    [self setPixelFormat:pf];
    
    [self setOpenGLContext:context];
    
#if SUPPORT_RETINA_RESOLUTION
    // Opt-In to Retina resolution
    [self setWantsBestResolutionOpenGLSurface:YES];
#endif // SUPPORT_RETINA_RESOLUTION
}

- (void) prepareOpenGL
{
    [super prepareOpenGL];
    
    // Make all the OpenGL calls to setup rendering
    //  and build the necessary rendering objects
    [self initGL];
    
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink);
    
    // Register to be notified when the window closes so we can stop the displaylink
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowWillClose:)
                                                 name:NSWindowWillCloseNotification
                                               object:[self window]];
}

- (void) windowWillClose:(NSNotification*)notification
{
    // Stop the display link when the window is closing because default
    // OpenGL render buffers will be destroyed.  If display link continues to
    // fire without renderbuffers, OpenGL draw calls will set errors.
    
    CVDisplayLinkStop(displayLink);
}

- (void) initGL
{
    // The reshape function may have changed the thread to which our OpenGL
    // context is attached before prepareOpenGL and initGL are called.  So call
    // makeCurrentContext to ensure that our OpenGL context current to this
    // thread (i.e. makeCurrentContext directs all OpenGL calls on this thread
    // to [self openGLContext])
    [[self openGLContext] makeCurrentContext];
    
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    // Create a vertex array object (VAO) to cache model parameters
    GLuint vaoName;
    glGenVertexArrays(1, &vaoName);
    glBindVertexArray(vaoName);
    
    BtTime::SetTick( 1.0f / 60.0f );
    
    NSString *resourceDirectory = [[NSBundle mainBundle] resourcePath];
    resourceDirectory = [resourceDirectory stringByAppendingString:@"/"];
    const BtChar *resources = [resourceDirectory cStringUsingEncoding:NSASCIIStringEncoding];
    ApConfig::SetResourcePath(resources);
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    documentsDirectory = [documentsDirectory stringByAppendingString:@"/"];
    const BtChar *documents = [documentsDirectory cStringUsingEncoding:NSASCIIStringEncoding];
    ApConfig::SetDocuments(documents);
    
    // Set the extension
    ApConfig::SetExtension( ".OSX" );
    
    // Set the platform
    ApConfig::SetPlatform( ApPlatform_GLES );
    
    // Create the renderer implementation
    RsImpl::pInstance()->Create();
    
    // Init the time
    BtTimeGLES::Init();
    
    // Destroy the sound manager
    SdSoundWinGL::CreateManager();
    
    // Create the project
    myProject.Create();
    
    // Get the view size in Points
    NSRect viewRectPoints = [self bounds];
    
#if SUPPORT_RETINA_RESOLUTION
    
    // Rendering at retina resolutions will reduce aliasing, but at the potential
    // cost of framerate and battery life due to the GPU needing to render more
    // pixels.
    
    // Any calculations the renderer does which use pixel dimentions, must be
    // in "retina" space.  [NSView convertRectToBacking] converts point sizes
    // to pixel sizes.  Thus the renderer gets the size in pixels, not points,
    // so that it can set it's viewport and perform and other pixel based
    // calculations appropriately.
    // viewRectPixels will be larger (2x) than viewRectPoints for retina displays.
    // viewRectPixels will be the same as viewRectPoints for non-retina displays
    NSRect viewRectPixels = [self convertRectToBacking:viewRectPoints];
    
#else //if !SUPPORT_RETINA_RESOLUTION
    
    // App will typically render faster and use less power rendering at
    // non-retina resolutions since the GPU needs to render less pixels.  There
    // is the cost of more aliasing, but it will be no-worse than on a Mac
    // without a retina display.
    
    // Points:Pixels is always 1:1 when not supporting retina resolutions
    NSRect viewRectPixels = viewRectPoints;
    
#endif // !SUPPORT_RETINA_RESOLUTION
    
    // Set the device pixel dimension in our renderer
    MtVector2 v2Dimension( viewRectPixels.size.width, viewRectPixels.size.height );
    RsImpl::pInstance()->SetDimension( v2Dimension );
    
    // Initialise the project
    myProject.Init();
    
    [self setupIdentity];
}

- (void) reshape
{
    [super reshape];
    
    // We draw on a secondary thread through the display link. However, when
    // resizing the view, -drawRect is called on the main thread.
    // Add a mutex around to avoid the threads accessing the context
    // simultaneously when resizing.
    CGLLockContext([[self openGLContext] CGLContextObj]);
    
    // Get the view size in Points
    NSRect viewRectPoints = [self bounds];
    
#if SUPPORT_RETINA_RESOLUTION
    
    // Rendering at retina resolutions will reduce aliasing, but at the potential
    // cost of framerate and battery life due to the GPU needing to render more
    // pixels.
    
    // Any calculations the renderer does which use pixel dimentions, must be
    // in "retina" space.  [NSView convertRectToBacking] converts point sizes
    // to pixel sizes.  Thus the renderer gets the size in pixels, not points,
    // so that it can set it's viewport and perform and other pixel based
    // calculations appropriately.
    // viewRectPixels will be larger (2x) than viewRectPoints for retina displays.
    // viewRectPixels will be the same as viewRectPoints for non-retina displays
    NSRect viewRectPixels = [self convertRectToBacking:viewRectPoints];
    
#else //if !SUPPORT_RETINA_RESOLUTION
    
    // App will typically render faster and use less power rendering at
    // non-retina resolutions since the GPU needs to render less pixels.  There
    // is the cost of more aliasing, but it will be no-worse than on a Mac
    // without a retina display.
    
    // Points:Pixels is always 1:1 when not supporting retina resolutions
    NSRect viewRectPixels = viewRectPoints;
    
#endif // !SUPPORT_RETINA_RESOLUTION
    
    // Set the device pixel dimension in our renderer
    MtVector2 v2Dimension( viewRectPixels.size.width, viewRectPixels.size.height );
    RsImpl::pInstance()->SetDimension( v2Dimension );
    
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
    
    if( v2Dimension.x )
    {
        // Resize the window
        myProject.Resize();
    }
}

- (void)renewGState
{
    // Called whenever graphics state updated (such as window resize)
    
    // OpenGL rendering is not synchronous with other rendering on the OSX.
    // Therefore, call disableScreenUpdatesUntilFlush so the window server
    // doesn't render non-OpenGL content in the window asynchronously from
    // OpenGL content, which could cause flickering.  (non-OpenGL content
    // includes the title bar and drawing done by the app with other APIs)
    [[self window] disableScreenUpdatesUntilFlush];
    
    [super renewGState];
}

- (void) drawRect: (NSRect) theRect
{
    // Called during resize operations
    
    // Avoid flickering during resize by drawiing
    [self drawView];
}

- (void) drawView
{
    [[self openGLContext] makeCurrentContext];
    
    // We draw on a secondary thread through the display link
    // When resizing the view, -reshape is called automatically on the main
    // thread. Add a mutex around to avoid the threads accessing the context
    // simultaneously when resizing
    CGLLockContext([[self openGLContext] CGLContextObj]);
    
    // Update identity
    if( ShIMU::GetNumSensors() )
    {
        if( PlayConfig == Identity1 )
        {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                [self updateIdentity];
            });
        }
        
        if( PlayConfig == Identity2 )
        {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                [self updateIdentity2];
            });
        }
    }
    
    // Update the input
    UiInputWinGL::Update();
    
    // Update the project
    myProject.Update();
    
    // Empty render targets
    RsImpl::pInstance()->EmptyRenderTargets();
    
    // Render the project
    myProject.Render();
    
    // Render
    RsImpl::pInstance()->Render();
    
    CGLFlushDrawable([[self openGLContext] CGLContextObj]);
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void) dealloc
{
    // Stop the display link BEFORE releasing anything in the view
    // otherwise the display link thread may call into the view and crash
    // when it encounters something that has been release
    CVDisplayLinkStop(displayLink);
    
    CVDisplayLinkRelease(displayLink);
    
    [super dealloc];
}

enum GameState
{
    GameState_Start,
    GameState_Red,
    GameState_Amber,
    GameState_Green,
    GameState_Battery,
    GameState_Identify,
};

const float Sensitivity = 0.2f;           // Define our sensitivity
const int Red   = 3;                        // How long do we have red on the screen
const int Green = 6;                        // How long do we have amber on the screen
const int Blue  = 7;                        // How long do we have green on the screen
const int RumbleAmount = 128;               // How much to rumble

long gameStart = 0;                             // When did the game start

int numControllers;                         // The number of controllers
int halfControllers;                        // Half the number of controllers
int wolf  = 0;                              // Which controller is the wolf
int wolf2 = 0;

bool moved  = false;                         // Has any controller been moved
bool moved2 = false;                         // Has any controller been moved
PSMove *pRumble = NULL;
PSMove *moveArr[16];                        // Get a handle to our controllers

#ifdef UseMadgwick
Madgwick madgwick[16];
#endif

// Game states
GameState gameState = GameState_Start;      // The gamestate

// last states
bool lastMoved   = moved;                    // Whether we moved
bool lastMoved2  = moved;                    // Whether we moved
GameState lastGameState;

-(void)setupIdentity
{
    numControllers = psmove_count_connected();
    
    ShIMU::SetNumSensors( numControllers );
    
    halfControllers = numControllers / 2;
    
    printf("Connected PS Move controllers: %d\n", numControllers );
    
    // Connect all the controllers
    for( int i=0; i<numControllers; i++)
    {
        moveArr[i] = psmove_connect_by_id(i);
        
        // Set the rumble to 0
        psmove_set_rumble( moveArr[i], 0 );
        
        // Set the lights to 0
        psmove_set_leds( moveArr[i], 0, 0, 0 );
        
        // Enable the orientation
        psmove_enable_orientation( moveArr[i], PSMove_True );
        PSMove_Bool isOrientation = psmove_has_orientation( moveArr[i] );
        (void)isOrientation;
        
        psmove_reset_orientation( moveArr[i] );
        
        int a=0;
        a++;
    }
}

-(void)updateIdentity
{
    if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_R ) )
    {
        for( BtU32 i=0; i<numControllers; i++ )
        {
        //    Madgwick &madge = madgwick[i];
        //    madge.q0 = 1.0f; madge.q1 = madge.q2 = madge.q3 = 0;
        }
    }
    
    // Get the current time
    time_t timer;
    long current = time(&timer);
    
    // How many elapsed seconds we have
    long elapsed = current - gameStart;
    
    // Handle the transition of game states
    if( gameState == GameState_Start )
    {
        // Generate the wolf
        wolf = rand() % numControllers;
        
        // Display the wolf
        printf("Starting the game with the wolf set to %d out of %d\n", wolf, numControllers);
        
        // Lets keep a timer
        time_t timer;
        gameStart = time( &timer );
        
        // Now turn the light to red
        gameState = GameState_Red;
    }
    else if( elapsed < Red )
    {
        gameState = GameState_Red;
    }
    else if( elapsed < Green )
    {
        gameState = GameState_Amber;
    }
    else if( elapsed < Blue )
    {
        gameState = GameState_Green;
    }
    else if( gameState == GameState_Green )
    {
        gameState = GameState_Identify;
    }
    
    // Do we turn rumble off
    if( pRumble )            // We had a rumble
    {
        printf("Rumble off\n");
        
        // Set the rumble
        psmove_set_rumble( pRumble, 0 );
        
        // Turned off
        pRumble = NULL;
    }
    
    // Shall we make some rumble
    if( lastGameState != gameState )
    {
        if( gameState == GameState_Green )
        {
            printf("Rumble on\n");
            
            // Cache each controller
            pRumble = moveArr[wolf];
            
            // Set the rumble
            psmove_set_rumble( pRumble, RumbleAmount );
        }
    }
    
    for( BtU32 i=0; i<numControllers; i++ )
    {
        PSMove *move = moveArr[i];
        
        int res = psmove_poll( move );
        if (res)
        {
            float fax, fay, faz;
            float fgx, fgy, fgz;
            
            MtQuaternion quaternion;
            
            // Are we using madgwick
#ifdef UseMadgwick
            
            Madgwick &madge = madgwick[i];
            
            for( BtU32 j=0; j<2; j++ )
            {
                PSMove_Frame frame = (PSMove_Frame)j;
                psmove_get_accelerometer_frame( move, frame, &fax, &fay, &faz );
                psmove_get_gyroscope_frame( move, frame, &fgx, &fgy, &fgz );
                madge.MadgwickAHRSupdateIMU( fgx, fgy, fgz, fax, fay, faz );
            }
            
            // Works vertically with x, z, y
            quaternion = MtQuaternion( -madge.q1, -madge.q3, -madge.q2, madge.q0 );
#else
            
            for( BtU32 j=0; j<2; j++ )
            {
                PSMove_Frame frame = (PSMove_Frame)j;
                psmove_get_accelerometer_frame( move, frame, &fax, &fay, &faz );
                psmove_get_gyroscope_frame( move, frame, &fgx, &fgy, &fgz );
            }
            
            BtFloat w, x, y, z;
            
            psmove_get_orientation( move, &w, &z, &y, &x );

            quaternion = MtQuaternion( -x, -z, -y, w );
#endif
            
            // Set the quaternion
            ShIMU::SetQuaternion( i, quaternion );
            
            // Construct the acceleration vector
            MtVector3 accel( fax, faz, fay );
            
            // Remove gravity
            MtVector3 frame( 0, -1, 0 );
            frame *= ShIMU::GetTransform(i).GetInverse();
            accel += frame;
            accel *= ShIMU::GetTransform(i);

            ShIMU::SetAccelerometer( i, accel );
            
            if( i == wolf )
            {
                moved = false;
                
#ifdef UseMadgwick
                // Was this controller moved
                if( ( MtAbs( accel.x ) > Sensitivity ) ||
                    ( MtAbs( accel.y ) > Sensitivity ) ||
                    ( MtAbs( accel.z ) > Sensitivity )
                   )
                {
                    // Yes this controller has been moved
                    moved = true;
                }
 #endif
                if( ( MtAbs( fgx ) > Sensitivity ) ||
                    ( MtAbs( fgy ) > Sensitivity ) ||
                    ( MtAbs( fgz ) > Sensitivity )
                  )
                {
                    // Yes this controller has been moved
                    moved = true;
                }
                
                // Should we restart the game? - if it's not being restarted
                if( gameState > GameState_Red )
                {
                    int trigger = psmove_get_trigger( move );
                    
                    if( trigger > 0 )//(128 + 64 + 32 + 16) )
                    {
                        gameState = GameState_Start;
                    }
                }
                
                // Check the battery level
                unsigned int pressed, released;
                psmove_get_button_events( move, &pressed, &released);
                
                if( pressed == Btn_MOVE )
                {
                    gameState = GameState_Battery;
                }
                else if( released == Btn_MOVE )
                {
                    gameState = GameState_Identify;
                }
            }
        }
    }
    
    // Lets only bother to change stuff if we have had a significant game change
    //   if( ( lastMoved != moved ) ||            // Something moved?
    //       ( lastGameState != gameState )       // Traffic lights changed colour
    //    )
    {
        // Keep a track of the moved state for efficency
        lastMoved = moved;
        
        // Set the new game state
        lastGameState = gameState;
        
        // Change the controllers all at once for speed
        for( int controllerIndex=0; controllerIndex<numControllers; controllerIndex++)
        {
            // Cache each controller
            PSMove *move = moveArr[controllerIndex];
            
            // Handle the game state and set the lights at once
            switch( gameState )
            {
                case GameState_Start:
                case GameState_Red:
                    psmove_set_leds( move, 255, 0, 0 );
                    break;
                    
                case GameState_Amber:
                    psmove_set_leds( move, 255, 100, 0 );
                    break;
                    
                case GameState_Green:
                    psmove_set_leds( move, 0, 255, 0 );
                    break;
                    
                case GameState_Identify:
                    
                    if( moved == true )
                    {
                        psmove_set_leds( move, 255, 255, 255 );
                    }
                    else
                    {
                        psmove_set_leds( move, 0, 0, 0 );
                    }
                    break;
                    
                case GameState_Battery:
                {
                    PSMove_Battery_Level batt = psmove_get_battery( move );
                    
                    switch( batt )
                    {
                        case Batt_MIN :
                            psmove_set_leds( move, 255, 0, 0 );
                            break;
                        case Batt_20Percent:
                            psmove_set_leds( move, 128, 128, 0 );
                            break;
                        case Batt_40Percent:
                            psmove_set_leds( move, 128, 200, 0 );
                            break;
                        case Batt_60Percent:
                            psmove_set_leds( move, 0, 200, 0 );
                            break;
                        case Batt_80Percent :
                            psmove_set_leds( move, 0, 220, 0 );
                            break;
                        case Batt_MAX:
                            psmove_set_leds( move, 0, 255, 0 );
                            break;
                        case Batt_CHARGING:
                            psmove_set_leds( move, 0, 0, 255 );
                            break;
                        case Batt_CHARGING_DONE:
                            psmove_set_leds( move, 255, 255, 255 );
                            break;
                    }
                    break;
                }
            }
            
            psmove_update_leds( move );
        }
    }
}

-(void)updateIdentity2
{
    if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_R ) )
    {
#ifdef UseMadgwick
        for( BtU32 i=0; i<numControllers; i++ )
        {
            Madgwick &madge = madgwick[i];
            madge.q0 = 1.0f; madge.q1 = madge.q2 = madge.q3 = 0;
        }
#endif
    }
    
    // Get the current time
    time_t timer;
    long current = time(&timer);
    
    // How many elapsed seconds we have
    long elapsed = current - gameStart;
    
    // Handle the transition of game states
    if( gameState == GameState_Start )
    {
        // Generate the wolves
        wolf  = rand() % halfControllers;
        wolf2 = rand() % halfControllers;
        wolf2 += halfControllers;
        
        // Display the wolf
        printf("Starting the game with the wolf set to %d out of %d\n", wolf, numControllers);
        
        // Lets keep a timer
        time_t timer;
        gameStart = time( &timer );
        
        // Now turn the light to red
        gameState = GameState_Red;
    }
    else if( elapsed < Red )
    {
        gameState = GameState_Red;
    }
    else if( elapsed < Green )
    {
        gameState = GameState_Amber;
    }
    else if( elapsed < Blue )
    {
        gameState = GameState_Green;
    }
    else if( gameState == GameState_Green )
    {
        gameState = GameState_Identify;
    }
    
    // Do we turn rumble off
    if( pRumble )            // We had a rumble
    {
        printf("Rumble off\n");
        
        // Set the rumble
        psmove_set_rumble( pRumble, 0 );
        
        // Turned off
        pRumble = NULL;
    }
    
    // Shall we make some rumble
    if( lastGameState != gameState )
    {
        if( gameState == GameState_Green )
        {
            printf("Rumble on\n");
            
            // Cache each controller
            pRumble = moveArr[wolf];
            
            // Set the rumble
            psmove_set_rumble( pRumble, RumbleAmount );
        }
    }
    
    for( BtU32 i=0; i<numControllers; i++ )
    {
        PSMove *move = moveArr[i];
        
        int res = psmove_poll( move );
        if (res)
        {
            float fax, fay, faz;
            float fgx, fgy, fgz;
            
            MtQuaternion quaternion;
            
#ifdef UseMadgwick
            
            Madgwick &madge = madgwick[i];
            
            for( BtU32 j=0; j<2; j++ )
            {
                PSMove_Frame frame = (PSMove_Frame)j;
                psmove_get_accelerometer_frame( move, frame, &fax, &fay, &faz );
                psmove_get_gyroscope_frame( move, frame, &fgx, &fgy, &fgz );
                madge.MadgwickAHRSupdateIMU( fgx, fgy, fgz, fax, fay, faz );
            }
            
            // Works vertically with x, z, y
            quaternion = MtQuaternion( -madge.q1, -madge.q3, -madge.q2, madge.q0 );
#else
            
            for( BtU32 j=0; j<2; j++ )
            {
                PSMove_Frame frame = (PSMove_Frame)j;
                psmove_get_accelerometer_frame( move, frame, &fax, &fay, &faz );
                psmove_get_gyroscope_frame( move, frame, &fgx, &fgy, &fgz );
            }
            
            BtFloat w, x, y, z;
            
            psmove_get_orientation( move, &w, &z, &y, &x );
            
            quaternion = MtQuaternion( -x, -z, -y, w );
#endif
            
            // Set the quaternion
            ShIMU::SetQuaternion( i, quaternion );
            
            // Construct the acceleration vector
            MtVector3 accel( fax, faz, fay );
            
            // Set the accelerometer
            ShIMU::SetAccelerometer( i, accel );
            
            if( i == wolf )
            {
                moved = false;
                
#ifdef UseMadgwick
                // Was this controller moved
                if( ( MtAbs( accel.x ) > Sensitivity ) ||
                    ( MtAbs( accel.y ) > Sensitivity ) ||
                    ( MtAbs( accel.z ) > Sensitivity )
                   )
                {
                    // Yes this controller has been moved
                    moved = true;
                }
 #endif
                
                if( ( MtAbs( fgx ) > Sensitivity ) ||
                    ( MtAbs( fgy ) > Sensitivity ) ||
                    ( MtAbs( fgz ) > Sensitivity )
                    )
                {
                    // Yes this controller has been moved
                    moved = true;
                }
                
                // Should we restart the game? - if it's not being restarted
                if( gameState > GameState_Red )
                {
                    int trigger = psmove_get_trigger( move );
                    
                    if( trigger > 0 )//(128 + 64 + 32 + 16) )
                    {
                        gameState = GameState_Start;
                    }
                }
                
                // Check the battery level
                unsigned int pressed, released;
                psmove_get_button_events( move, &pressed, &released);
                
                if( pressed == Btn_MOVE )
                {
                    gameState = GameState_Battery;
                }
                else if( released == Btn_MOVE )
                {
                    gameState = GameState_Identify;
                }
            }
            
            if( i == wolf2 )
            {
                moved2 = false;
                
                // Was this controller moved
                if( ( MtAbs( accel.x ) > Sensitivity ) ||
                   ( MtAbs( accel.y ) > Sensitivity ) ||
                   ( MtAbs( accel.z ) > Sensitivity )
                   )
                {
                    // Yes this controller has been moved
                    moved2 = true;
                }
                else if( ( MtAbs( fgx ) > Sensitivity ) ||
                        ( MtAbs( fgy ) > Sensitivity ) ||
                        ( MtAbs( fgz ) > Sensitivity )
                        )
                {
                    // Yes this controller has been moved
                    moved2 = true;
                }
                
                // Should we restart the game? - if it's not being restarted
                if( gameState > GameState_Red )
                {
                    int trigger = psmove_get_trigger( move );
                    
                    if( trigger > 0 )//(128 + 64 + 32 + 16) )
                    {
                        gameState = GameState_Start;
                    }
                }
                
                // Check the battery level
                unsigned int pressed, released;
                psmove_get_button_events( move, &pressed, &released);
                
                if( pressed == Btn_MOVE )
                {
                    gameState = GameState_Battery;
                }
                else if( released == Btn_MOVE )
                {
                    gameState = GameState_Identify;
                }
            }
        }
        
        // Keep a track of the moved state for efficency
        lastMoved  = moved;
        lastMoved2 = moved2;
        
        // Set the new game state
        lastGameState = gameState;
        
        // Change the controllers all at once for speed
        for( int controllerIndex=0; controllerIndex<numControllers; controllerIndex++)
        {
            // Cache each controller
            PSMove *move = moveArr[controllerIndex];
            
            // Handle the game state and set the lights at once
            switch( gameState )
            {
                case GameState_Start:
                case GameState_Red:
                    psmove_set_leds( move, 255, 0, 0 );
                    break;
                    
                case GameState_Amber:
                    psmove_set_leds( move, 255, 100, 0 );
                    break;
                    
                case GameState_Green:
                    psmove_set_leds( move, 0, 255, 0 );
                    break;
                    
                case GameState_Identify:
                    
                    if( controllerIndex < halfControllers )
                    {
                        if( moved == true )
                        {
                            psmove_set_leds( move, 0xaa, 0x52, 0x39 );
                        }
                        else
                        {
                            psmove_set_leds( move, 0, 0, 0 );
                        }
                    }
                    else
                    {
                        if( moved2 == true )
                        {
                            psmove_set_leds( move, 0x25, 0x5c, 0x69 );
                        }
                        else
                        {
                            psmove_set_leds( move, 0, 0, 0 );
                        }
                    }
                    break;
                    
                case GameState_Battery:
                {
                    PSMove_Battery_Level batt = psmove_get_battery( move );
                    
                    switch( batt )
                    {
                        case Batt_MIN :
                            psmove_set_leds( move, 255, 0, 0 );
                            break;
                        case Batt_20Percent:
                            psmove_set_leds( move, 128, 128, 0 );
                            break;
                        case Batt_40Percent:
                            psmove_set_leds( move, 128, 200, 0 );
                            break;
                        case Batt_60Percent:
                            psmove_set_leds( move, 0, 200, 0 );
                            break;
                        case Batt_80Percent :
                            psmove_set_leds( move, 0, 220, 0 );
                            break;
                        case Batt_MAX:
                            psmove_set_leds( move, 0, 255, 0 );
                            break;
                        case Batt_CHARGING:
                            psmove_set_leds( move, 0, 0, 255 );
                            break;
                        case Batt_CHARGING_DONE:
                            psmove_set_leds( move, 255, 255, 255 );
                            break;
                    }
                    break;
                }
            }
        }
        
        // Change the controllers all at once for speed
        for( int controllerIndex=0; controllerIndex<numControllers; controllerIndex++)
        {
            // Cache each controller
            PSMove *move = moveArr[controllerIndex];
            
            // Always update our leds
            psmove_update_leds( move );
        }
    }
}

@end
