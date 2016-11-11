import { NgModule } from '@angular/core';
import { RouterModule } from '@angular/router';
import { UniversalModule } from 'angular2-universal';
import { AppComponent } from './components/app/app.component'
import { ProfileComponent } from './components/profile/profile.component';
import { CadComponent } from './components/cad/cad.component';
import { HeaderbarComponent } from './components/headerbar/headerbar.component';
import { WorkspaceComponent } from './components/workspace/workspace.component';
import { ModelsService } from './services/models/models.service'

@NgModule({
    bootstrap: [ AppComponent ],
    declarations: [
        AppComponent,
        CadComponent,
        ProfileComponent,
        HeaderbarComponent,
        WorkspaceComponent
    ],
    providers: [
        ModelsService,
    ],
    imports: [
        UniversalModule, // Must be first import. This automatically imports BrowserModule, HttpModule, and JsonpModule too.
        RouterModule.forRoot([
            { path: '', redirectTo: 'cad', pathMatch: 'full' },
            { path: 'cad', component: CadComponent },
            { path: 'profile', component: ProfileComponent },
            { path: '**', redirectTo: 'cad' }
        ])
    ]
})
export class AppModule {
}
