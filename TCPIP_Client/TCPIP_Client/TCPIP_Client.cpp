/ /   T C P I P _ C l i e n t . c p p   :   T h i s   f i l e   c o n t a i n s   t h e   ' m a i n '   f u n c t i o n .   P r o g r a m   e x e c u t i o n   b e g i n s   a n d   e n d s   t h e r e .  
 / /  
 # p r a g m a   c o m m e n t   ( l i b ,   " W s 2 _ 3 2 . l i b " )  
  
  
 # i n c l u d e   < i o s t r e a m >  
 # i n c l u d e   < W i n S o c k 2 . h >  
 # i n c l u d e   < W S 2 t c p i p . h >  
 # i n c l u d e   < s t d i o . h >  
 # i n c l u d e   < s t r i n g >  
 # i n c l u d e   < r a n d o m >  
 # i n c l u d e   < t h r e a d >  
 # i n c l u d e   " . . / . . / P r o t o c o l A P I / P r o t o c o l A P I / M e s s a g e . h "  
 # i n c l u d e   < f u t u r e >  
  
 / / # i n c l u d e   < s y s / t y p e s . h >  
 / / # i n c l u d e   < s y s / s o c k e t . h >  
  
 # d e f i n e   M A X _ P O R T _ D I G I T   5  
  
 c l a s s   U I I n t e r f a c e  
 {  
 p u b l i c :  
 	 s t d : : s t r i n g   G e t N a m e ( )  
 	 {  
 	 	 s t d : : s t r i n g   r e t ;  
 	 	 s t d : : c o u t   < <   " E n t e r   y o u r   n a m e :   \ n " ;  
 	 	 s t d : : c i n   > >   r e t ;  
 	 	 r e t u r n     r e t ;  
 	 }  
 	 s t d : : s t r i n g   G e t I P ( )  
 	 {  
 	 	 s t d : : c o u t   < <   " E n t e r   S e r v e r   I P   o r   p r e s s   E n t e r   f o r   u s e   l o c a l h o s t   \ n "   < <   s t d : : e n d l ;  
  
 	 	 / / T O D O   D o   w e   h a v e   b e s t   c l e a n   \ n   m e t h o d ?  
 	 	 s t d : : c i n . i g n o r e ( ) ;  
  
 	 	 / / T O D O   c h e c k    
 	 	 s t d : : s t r i n g   r e t ;  
 	 	 s t d : : g e t l i n e ( s t d : : c i n ,   r e t ) ;  
  
 	 	 / / T O D O   c h e c k  
 	 	 i f   ( r e t . l e n g t h ( )   = =   0 )  
 	 	 {  
 	 	 	 r e t   =   " 1 2 7 . 0 . 0 . 1 " ;  
 	 	 }  
 	 	 e l s e   i f   ( r e t . l e n g t h ( )   >   I N E T _ A D D R S T R L E N )  
 	 	 {  
 	 	 	 t h r o w   s t d : : e x c e p t i o n ( " I n v a l i d   I P v 4   a d d r e s s " ) ;  
 	 	 }  
  
 	 	 r e t u r n     r e t ;  
 	 }  
 	 s t d : : s t r i n g   G e t P o r t ( )  
 	 {  
 	 	 s t d : : s t r i n g   r e t ;  
 	 	 s t d : : c o u t   < <   " E n t e r   S e r v e r   P o r t   o r   p r e s s   E n t e r   f o r   u s e   d e f a u l t   7 7 0 0   \ n "   < <   s t d : : e n d l ;  
 	 	 s t d : : g e t l i n e ( s t d : : c i n ,   r e t ) ;  
  
 	 	 / / T O D O   c h e c k  
 	 	 i f   ( r e t . l e n g t h ( )   = =   0 )  
 	 	 {  
 	 	 	 r e t   =   " 7 7 0 0 " ;  
 	 	 }  
 	 	 e l s e   i f   ( r e t . l e n g t h ( )   >   M A X _ P O R T _ D I G I T )  
 	 	 {  
 	 	 	 t h r o w   s t d : : e x c e p t i o n ( " I n v a l i d   I P v 4   p o r t " ) ;  
 	 	 }  
 	 	 r e t u r n     r e t ;  
 	 }  
 	 v o i d   P r i n t M e s s a g e ( s t d : : s t r i n g   s t r )  
 	 {  
 	 	 s t d : : c o u t   < <   s t r ;  
 	 }  
 	 v o i d   P r i n t M e s s a g e ( C h a t L i b : : M e s s a g e   m s g )  
 	 {  
 	 	 s t d : : c o u t   < <   m s g . G e t T e x t ( ) ;  
 	 }  
 } ;  
  
 	 c l a s s   T C P I P _ C l i e n t  
 	 {  
 	 p u b l i c :  
  
 	 	 s t d : : s t r i n g   N a m e ;  
 	 	 s t d : : s t r i n g   S e r v e r I P ;  
 	 	 s t d : : s t r i n g   S e r v e r P o r t ;  
 	 	 i n t   S o c k e t   =   N U L L ;  
 	 	 / / R e s p o n s e   T r y R e c i e v e M e s s a g e ( c h a r * ) ;  
  
 	 	 / / v o i d   C a l l I n i t i a l i z e D i a l o g ( c h a r   *   n a m e B u f f ,   c h a r   *   s e r v e r I P b u f f ,   c h a r   *   s e r v e r P o r t B u f f ) ;  
  
 	 	 / / i n t   I n i t i a l i z e S o c k e t R o u t i n e ( ) ;  
  
 	 	 / / T O D O   w h y   c o n s t   a n d   &    
 	 	 / / T O D O   d o n ` t   c o p y ?  
 	 	 / / R e s p o n s e   T r y S e n d M e s s a g e ( c o n s t   s t d : : s t r i n g &   c s ) ;  
 	 	  
 	 	 T C P I P _ C l i e n t ( s t d : : s t r i n g   n a m e ,   s t d : : s t r i n g   s e r v e r I P ,   s t d : : s t r i n g   s e r v e r P o r t )  
 	 	 {  
 	 	 	 N a m e   =   n a m e ;  
 	 	 	 S e r v e r I P   =   s e r v e r I P ;  
 	 	 	 S e r v e r P o r t   =   s e r v e r P o r t ;  
 	 	 }  
  
 	 	 C h a t L i b : : M e s s a g e T y p e   I n t r o d u c e T o S e r v e r ( )  
 	 	 {  
 	 	 	 i f   ( N a m e . l e n g t h ( )   = =   0 )  
 	 	 	 	 t h r o w   n e w   s t d : : e x c e p t i o n ( " Y o u   m u s t   f i l l   c l i e n t . N a m e   b e f o r e   c a l l   I n t r o d u c e T o S e r v e r ( )   \ n " ) ;  
  
 	 	 	 C h a t L i b : : M e s s a g e   m e s s a g e ( C h a t L i b : : e N a m e R e q u e s t ,   & N a m e ) ;  
  
 	 	 	 C h a t L i b : : M e s s a g e T y p e   m s g T y p e ;  
 	 	 	 d o  
 	 	 	 {  
 	 	 	 	 m s g T y p e   =   C h a t L i b : : P r o t o c o l : : T r y S e n d M e s s a g e ( m e s s a g e ,   S o c k e t ) ;  
 	 	 	 }   w h i l e   ( m s g T y p e   ! =   C h a t L i b : : e R e s p o n s e O k ) ;  
 	 	 	 r e t u r n   m s g T y p e ;  
 	 	 }  
  
 	 	 v o i d   I n i t i a l i z e S o c k e t R o u t i n e ( )  
 	 	 {  
 	 	 	 W S A D A T A   w s a D a t a ;  
  
 	 	 	 i f   ( W S A S t a r t u p ( M A K E W O R D ( 2 ,   2 ) ,   & w s a D a t a ) )  
 	 	 	 {  
 	 	 	 	 s t d : : c o u t   < <   " C o u l d   n o t   i n i t i a l i z e   w i n s o c k   \ n "   < <   s t d : : e n d l ;  
 	 	 	 }  
  
 	 	 	 s t r u c t   a d d r i n f o   h i n t s ;  
 	 	 	 s t r u c t   a d d r i n f o   * s e r v i n f o ;  
  
 	 	 	 m e m s e t ( & h i n t s ,   0 ,   s i z e o f   h i n t s ) ;  
  
 	 	 	 h i n t s . a i _ f a m i l y   =   P F _ I N E T ;  
 	 	 	 h i n t s . a i _ s o c k t y p e   =   S O C K _ S T R E A M ;  
 	 	 	 h i n t s . a i _ p r o t o c o l   =   I P P R O T O _ T C P ;  
 	 	 	 / / h i n t s . a i _ f l a g s   =   A I _ P A S S I V E ;  
  
  
 	 	 	 / / T O D O   c h e c k   e r r o r s  
 	 	 	 i f   ( i n t   s t a t u s   =   g e t a d d r i n f o ( S e r v e r I P . c _ s t r ( ) ,   S e r v e r P o r t . c _ s t r ( ) ,   & h i n t s ,   & s e r v i n f o )   ! =   0 )  
 	 	 	 {  
 	 	 	 	 s t d : : c o u t   < <   " g e t a d d r i n f o   e r r o r :   \ n "   < <   g a i _ s t r e r r o r ( s t a t u s )   < <   s t d : : e n d l ;  
 	 	 	 }  
  
  
 	 	 	 S o c k e t   =   s o c k e t ( s e r v i n f o - > a i _ f a m i l y ,   s e r v i n f o - > a i _ s o c k t y p e ,   s e r v i n f o - > a i _ p r o t o c o l ) ;  
  
 	 	 	 / / T O D O   m a y b e   n e e d   b i n d  
  
 	 	 	 / / c o n n e c t ( s o c k f d ,   s e r v i n f o - > a i _ a d d r ,   s e r v i n f o - > a i _ a d d r l e n ) ;  
 	 	 	 i f   ( i n t   s t a t u s   =   c o n n e c t ( S o c k e t ,   s e r v i n f o - > a i _ a d d r ,   s e r v i n f o - > a i _ a d d r l e n )   ! =   0 )  
 	 	 	 {  
 	 	 	 	 s t d : : c o u t   < <   " c o n n e c t   e r r o r :   "   < <   g a i _ s t r e r r o r ( s t a t u s )   < <   s t d : : e n d l ;  
 	 	 	 }  
  
 	 	 }  
  
 	 	 i n t   G e t S o c k e t ( )  
 	 	 {  
 	 	 	 r e t u r n   S o c k e t ;  
 	 	 }  
 	 } ;  
 	   v o i d   U s e r I n p u t ( s t d : : s t r i n g   & m e s s a g e )  
 	 {  
 	 	 s t d : : c i n   > >   m e s s a g e ;  
 	 }  
 	   s t d : : s t r i n g   G e t U s e r I n p u t ( )  
 	   {  
 	 	   s t d : : s t r i n g   s ;  
 	 	   s t d : : c i n   > >   s ;  
 	 	   r e t u r n   s ;  
 	   }  
  
 / / T O D O   c h e c k  
 	   i n t   m a i n ( )  
 	   {  
 	 	   s e t l o c a l e ( L C _ A L L ,   " R u s s i a n " ) ;  
  
 	 	   U I I n t e r f a c e   u i ;  
 	 	   s t d : : s t r i n g   c o n f C l i e n t N a m e   =   u i . G e t N a m e ( ) ;  
 	 	   s t d : : s t r i n g   c o n f S e r v e r I P   =   u i . G e t I P ( ) ;  
 	 	   s t d : : s t r i n g   c o n f S e r v e r P o r t   =   u i . G e t P o r t ( ) ;  
  
 	 	   T C P I P _ C l i e n t   c l i e n t ( c o n f C l i e n t N a m e ,   c o n f S e r v e r I P ,   c o n f S e r v e r P o r t ) ;  
  
 	 	   c l i e n t . I n i t i a l i z e S o c k e t R o u t i n e ( ) ;  
 	 	   i n t   s o c k f d   =   c l i e n t . G e t S o c k e t ( ) ;  
  
 	 	   c l i e n t . I n t r o d u c e T o S e r v e r ( ) ;  
  
 	 	   c h a r   b u f f [ 1 0 2 4 ] ;  
  
 	 	   w h i l e   ( t r u e )  
 	 	   {  
 	 	 	   / / T O D O   h o w   t o   d e l e t e   r i g h t  
 	 	 	   C h a t L i b : : M e s s a g e   m e s s a g e   =   C h a t L i b : : P r o t o c o l : : T r y R e c i e v e M e s s a g e ( s o c k f d ) ;  
  
 	 	 	   i f   ( m e s s a g e . G e t T e x t ( ) . l e n g t h ( )   ! =   0 )  
 	 	 	 	   u i . P r i n t M e s s a g e ( m e s s a g e ) ;  
  
 	 	 	   / / s t d : : c h r o n o : : m i l l i s e c o n d s   t i m e ( 1 0 0 ) ;  
  
 	 	 	   / / s t d : : f u t u r e < s t d : : s t r i n g >   f u t u r e   =   s t d : : a s y n c ( G e t U s e r I n p u t ) ;  
  
 	 	 	   / / w h i l e ( f u t u r e . w a i t _ f o r ( t i m e )   = =   s t d : : f u t u r e _ s t a t u s : : t i m e o u t )  
  
 	 	 	   a u t o   f   =   s t d : : a s y n c (   s t d : : l a u n c h : : a s y n c ,   [ ]  
 	 	 	   {  
 	 	 	 	   s t d : : s t r i n g   s   =   " " ;  
 	 	 	 	   i f   ( s t d : : c i n   > >   s )   r e t u r n   s ;  
 	 	 	   } ) ;  
  
 	 	 	   s t d : : s t r i n g   s t r ;  
 	 	 	   / / i f ( f . w a i t _ f o r ( s t d : : c h r o n o : : s e c o n d s ( 0 ) )   = =   s t d : : f u t u r e _ s t a t u s : : r e a d y )  
 	 	 	   / / T O D O   c h e c k   f u t u r e   s t a t u s   b e f o r e   a c c e s s  
 	 	 	   i f ( f . v a l i d ( ) )  
 	 	 	   {  
 	 	 	 	   s t r   =   f . g e t ( ) ;  
 	 	 	   }  
 	 	 	   e l s e  
 	 	 	   {  
 	 	 	 	   s t r   =   "   T e s t   m e s s a g e   \ n " ;  
 	 	 	   }  
  
 	 	 	   C h a t L i b : : M e s s a g e   s e n d M e s s a g e ( C h a t L i b : : e M e s s a g e R e q u e s t ,   & s t r ) ;  
  
 	 	 	   C h a t L i b : : M e s s a g e T y p e   r e s p o n s e   =   C h a t L i b : : P r o t o c o l : : T r y S e n d M e s s a g e ( s e n d M e s s a g e ,   s o c k f d ) ;  
  
 	 	 	   / / s t d : : s t r i n g   m e s s a g e ;  
 	 	 	   / / s t d : : t h r e a d   t h r e a d ( U s e r I n p u t ,   s t d : : r e f ( m e s s a g e ) ) ;  
 	 	 	   / / t h r e a d . d e t a c h ( ) ;  
  
 	 	   }  
  
 	 	   c l o s e s o c k e t ( s o c k f d ) ;  
 	 	   s t d : : c o u t   < <   " H e l l o   W o r l d ! \ n " ;  
  
 	 	   r e t u r n   0 ;  
 	   }  
  
 	 / /   R u n   p r o g r a m :   C t r l   +   F 5   o r   D e b u g   >   S t a r t   W i t h o u t   D e b u g g i n g   m e n u  
 	 / /   D e b u g   p r o g r a m :   F 5   o r   D e b u g   >   S t a r t   D e b u g g i n g   m e n u  
  
 	 / /   T i p s   f o r   G e t t i n g   S t a r t e d :    
 	 / /       1 .   U s e   t h e   S o l u t i o n   E x p l o r e r   w i n d o w   t o   a d d / m a n a g e   f i l e s  
 	 / /       2 .   U s e   t h e   T e a m   E x p l o r e r   w i n d o w   t o   c o n n e c t   t o   s o u r c e   c o n t r o l  
 	 / /       3 .   U s e   t h e   O u t p u t   w i n d o w   t o   s e e   b u i l d   o u t p u t   a n d   o t h e r   m e s s a g e s  
 	 / /       4 .   U s e   t h e   E r r o r   L i s t   w i n d o w   t o   v i e w   e r r o r s  
 	 / /       5 .   G o   t o   P r o j e c t   >   A d d   N e w   I t e m   t o   c r e a t e   n e w   c o d e   f i l e s ,   o r   P r o j e c t   >   A d d   E x i s t i n g   I t e m   t o   a d d   e x i s t i n g   c o d e   f i l e s   t o   t h e   p r o j e c t  
 	 / /       6 .   I n   t h e   f u t u r e ,   t o   o p e n   t h i s   p r o j e c t   a g a i n ,   g o   t o   F i l e   >   O p e n   >   P r o j e c t   a n d   s e l e c t   t h e   . s l n   f i l e  
  
  
  
 