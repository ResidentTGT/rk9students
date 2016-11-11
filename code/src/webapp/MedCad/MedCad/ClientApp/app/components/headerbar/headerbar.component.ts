import { Component } from '@angular/core';
import { ModelsService } from '../../services/models/models.service'

@Component({
    selector: 'headerbar',
    template: require('./headerbar.component.html'),
    styles: [require('./headerbar.component.scss')],
    providers: [ModelsService]
})
export class HeaderbarComponent {
    private openedFile: any;

    constructor(private modelsService: ModelsService) { }

    public openFileContextClickHandler() {
        document.getElementById('open-file-dlg').click();
    }

    public openedFileChanged(fileInput: any) {
        this.modelsService.openFile(fileInput.target.files[0]);
    }
}
