#include <allegro.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    //initialize Allegro
	allegro_init();
    set_gfx_mode(GFX_SAFE, 640, 480, 0, 0);
    install_keyboard();
    
    //display version info
	textprintf_ex(screen, font, 0, 0, 15, -1, 
        "Allegro version:  %s", allegro_id);

    int year = ALLEGRO_DATE / 10000;
    int month = (ALLEGRO_DATE / 100) % 100;
    int day = ALLEGRO_DATE % 100;
    textprintf_ex(screen,font,0,20,15,-1,
        "Revision Date:    %d/%d/%d", month, day, year);

    //display operating system
    char os[20];
    switch (os_type) {
        case OSTYPE_UNKNOWN: strcpy(os,"Unknown/DOS");     break;
        case OSTYPE_WIN3:    strcpy(os,"Windows 3.1");     break;
        case OSTYPE_WIN95:   strcpy(os,"Windows 95");      break;
        case OSTYPE_WIN98:   strcpy(os,"Windows 98");      break;
        case OSTYPE_WINME:   strcpy(os,"Windows ME");      break;
        case OSTYPE_WINNT:   strcpy(os,"Windows NT");      break;
        case OSTYPE_WIN2000: strcpy(os,"Windows 2000");    break;
        case OSTYPE_WINXP:   strcpy(os,"Windows XP");      break;
        case OSTYPE_OS2:     strcpy(os,"OS/2");            break;
        case OSTYPE_WARP:    strcpy(os,"OS/2 Warp 3");     break;
        case OSTYPE_DOSEMU:  strcpy(os,"Linux DOSEMU");    break;
        case OSTYPE_OPENDOS: strcpy(os,"Caldera OpenDOS"); break;
        case OSTYPE_LINUX:   strcpy(os,"Linux");           break;
        case OSTYPE_SUNOS:   strcpy(os,"SunOS/Solaris");   break;
        case OSTYPE_FREEBSD: strcpy(os,"FreeBSD");         break;
        case OSTYPE_NETBSD:  strcpy(os,"NetBSD");          break;
        case OSTYPE_IRIX:    strcpy(os,"IRIX");            break;
        case OSTYPE_DARWIN:  strcpy(os,"Darwin");          break;
        case OSTYPE_QNX:     strcpy(os,"QNX");             break;
        case OSTYPE_UNIX:    strcpy(os,"Unix");            break;
        case OSTYPE_BEOS:    strcpy(os,"BeOS");            break;
        case OSTYPE_MACOS:   strcpy(os,"MacOS");           break;
        case OSTYPE_MACOSX:  strcpy(os,"MacOS X");         break;
    }
    //display version tacked onto end of O/S name
    textprintf_ex(screen,font,0,40,15,-1,
        "Operating System: %s %i.%i", os, os_version, os_revision);

    if (os_multitasking == 0) 
        textout_ex(screen,font,"Multitask:        No",0,60,15,-1);
    else
        textout_ex(screen,font,"Multitask:        Yes",0,60,15,-1);
	
	//display system info
    int width, height;
    if (get_desktop_resolution(&width, &height) != 0)
	   textout_ex(screen,font,"Resolution: Unknown",0,80,15,-1);
    else
        textprintf_ex(screen,font,0,80,15,-1,
            "Resolution:       %i x %i", width, height);

    textprintf_ex(screen,font,0,100,15,-1,
        "Color Depth:      %i bits", desktop_color_depth());

    //display CPU information
    textprintf_ex(screen,font,0,120,15,-1,
        "CPU Vendor:       %s", cpu_vendor);

    //for each cpu family, check the specific model number
    char family[40], model[40];
    switch (cpu_family) {
        case CPU_FAMILY_I386: 
            strcpy(family, "386");
            sprintf(model, "%i", cpu_model);
            break;

        case CPU_FAMILY_I486:
            strcpy(family, "486");
            switch (cpu_model) {
                case CPU_MODEL_I486DX:   
                    strcpy(model,"486 DX");    
                    break;
                case CPU_MODEL_I486DX50: 
                    strcpy(model,"486 DX/50"); 
                    break;
                case CPU_MODEL_I486SX:   
                    strcpy(model,"486 SX");    
                    break;
                case CPU_MODEL_I487SX:   
                    strcpy(model,"487 SX");    
                    break;
                case CPU_MODEL_I486SL:   
                    strcpy(model,"486 SL");    
                    break;
                case CPU_MODEL_I486SX2:  
                    strcpy(model,"486 SX/2");  
                    break;
                case CPU_MODEL_I486DX2:  
                    strcpy(model,"486 DX/2");  
                    break;
                case CPU_MODEL_I486DX4:  
                    strcpy(model,"486 DX/4");  
                    break;
            }
            break;

        case CPU_FAMILY_I586:
            strcpy(family, "586");
            switch (cpu_model) {
                case CPU_MODEL_K5: 
                    strcpy(model,"AMD K5"); 
                    break;
                case CPU_MODEL_PENTIUM: 
                    strcpy(model,"Pentium"); 
                    break;
                case CPU_MODEL_PENTIUMP54C: 
                    strcpy(model,"Pentium Pro"); 
                    break;
                case CPU_MODEL_PENTIUMOVERDRIVE: 
                    strcpy(model,"Pentium Overdrive"); 
                    break;
                case CPU_MODEL_PENTIUMOVERDRIVEDX4: 
                    strcpy(model,"Pentium Overdrive DX/4"); 
                    break;
                case CPU_MODEL_K6: 
                    strcpy(model,"AMD K6"); 
                    break;
                case CPU_MODEL_CYRIX: 
                    strcpy(model,"Cyrix"); 
                    break;
                case CPU_MODEL_UNKNOWN: 
                    sprintf(model,"%i",cpu_model); 
                    break;
                default: 
                    sprintf(model,"%i",cpu_model); 
                    break;
            }
            break;

        case CPU_FAMILY_I686:
            strcpy(family, "686");
            switch (cpu_model) {
                case CPU_MODEL_PENTIUMPROA: 
                    strcpy(model,"Pentium Pro/A"); 
                    break;
                case CPU_MODEL_PENTIUMPRO: 
                    strcpy(model,"Pentium Pro"); 
                    break;
                case CPU_MODEL_PENTIUMIIKLAMATH: 
                    strcpy(model,"Pentium II/Klamath"); 
                    break;
                case CPU_MODEL_PENTIUMII: 
                    strcpy(model,"Pentium II"); 
                    break;
                case CPU_MODEL_CELERON: 
                    strcpy(model,"Celeron"); 
                    break;
                case CPU_MODEL_PENTIUMIIIKATMAI: 
                    strcpy(model,"Pentium III/Katmai"); 
                    break;
                case CPU_MODEL_PENTIUMIIICOPPERMINE: 
                    strcpy(model,"Pentium III/Coppermine"); 
                    break;
                case CPU_MODEL_PENTIUMIIIMOBILE: 
                    strcpy(model,"Pentium III/Mobile"); 
                    break;
                case CPU_MODEL_ATHLON: 
                    strcpy(model,"Athlon"); 
                    break;
                default: 
                    sprintf(model,"%i",cpu_model); 
                    break;
            }
            break;

        case CPU_FAMILY_ITANIUM:
            strcpy(family, "Itanium");
            sprintf(model, "%i", cpu_model);
            break;

        case CPU_FAMILY_EXTENDED:
            sprintf(family,"%i (unknown)", cpu_family);
            switch (cpu_model) {
                case CPU_MODEL_PENTIUMIV: 
                    strcpy(model,"Pentium IV"); 
                    break;
                case CPU_MODEL_XEON: 
                    strcpy(model,"Pentium IV Xeon"); 
                    break;
                case CPU_MODEL_ATHLON64: 
                    strcpy(model,"AMD Athlon 64"); 
                    break;
                case CPU_MODEL_OPTERON: 
                    strcpy(model,"AMD Opteron"); 
                    break;
                case 14: 
                    strcpy(model,"Pentium IV HyperThreading"); 
                    break;
                default: 
                    sprintf(model,"%i (unknown)", cpu_model); 
                    break;
             }
             break;
     
         case CPU_FAMILY_POWERPC:
            strcpy(family, "PowerPC");
             switch(cpu_model) {
                 case CPU_MODEL_POWERPC_601:   
                     strcpy(model,"601");   
                     break;
                 case CPU_MODEL_POWERPC_602:   
                     strcpy(model,"602");   
                     break;
                 case CPU_MODEL_POWERPC_603:   
                     strcpy(model,"603");   
                     break;
                 case CPU_MODEL_POWERPC_603e:  
                     strcpy(model,"603e");  
                     break;
                 case CPU_MODEL_POWERPC_603ev: 
                     strcpy(model,"603ev"); 
                     break;
                 case CPU_MODEL_POWERPC_604:   
                     strcpy(model,"604");   
                     break;
                 case CPU_MODEL_POWERPC_604e:  
                     strcpy(model,"604e");  
                     break;
                 case CPU_MODEL_POWERPC_620:   
                     strcpy(model,"620");   
                     break;
                 case CPU_MODEL_POWERPC_750:   
                     strcpy(model,"750");   
                     break;
                 case CPU_MODEL_POWERPC_7400:  
                     strcpy(model,"7400");  
                     break;
                 case CPU_MODEL_POWERPC_7450:  
                     strcpy(model,"7450");  
                     break;
                default: 
                    sprintf(model,"%i",cpu_model); 
                    break;
             break;
             }
        case CPU_FAMILY_UNKNOWN: 
            sprintf(family, "%i", cpu_family);
            sprintf(model, "%i", cpu_model);
            break;
    }

    textprintf_ex(screen,font,0,140,15,-1,
        "CPU Family:       %s", family);
    textprintf_ex(screen,font,0,160,15,-1,
        "CPU Model:        %s", model);
    

    //display processor capabilities
    char caps[40];
    strcpy(caps,"");
    if ((cpu_capabilities & CPU_ID)==CPU_ID) 
        strcat(caps,"CPU_ID ");
    if ((cpu_capabilities & CPU_FPU)==CPU_FPU) 
        strcat(caps,"FPU "); 
    if ((cpu_capabilities & CPU_MMX)==CPU_MMX) 
        strcat(caps,"MMX "); 
    if ((cpu_capabilities & CPU_MMXPLUS)==CPU_MMXPLUS) 
        strcat(caps,"MMX+ "); 
    if ((cpu_capabilities & CPU_SSE)==CPU_SSE) 
        strcat(caps,"SSE "); 
    if ((cpu_capabilities & CPU_SSE2)==CPU_SSE2) 
        strcat(caps,"SSE2 "); 
    if ((cpu_capabilities & CPU_SSE3)==CPU_SSE3) 
        strcat(caps,"SSE3 "); 
    if ((cpu_capabilities & CPU_3DNOW)==CPU_3DNOW) 
        strcat(caps,"3DNOW "); 
    if ((cpu_capabilities & CPU_ENH3DNOW)==CPU_ENH3DNOW) 
        strcat(caps,"ENH_3DNOW "); 
    if ((cpu_capabilities & CPU_CMOV)==CPU_CMOV) 
        strcat(caps,"CMOV "); 
    if ((cpu_capabilities & CPU_AMD64)==CPU_AMD64) 
        strcat(caps,"AMD64 "); 
    if ((cpu_capabilities & CPU_IA64)==CPU_IA64) 
        strcat(caps,"IA64 "); 

    textprintf_ex(screen,font,0,180,15,-1,
        "CPU Capabilities: %s", caps);

    while(!keypressed());
    allegro_exit();
	return 0;     
}     
END_OF_MAIN()
