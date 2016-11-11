import { Injectable } from '@angular/core';

@Injectable()
export class ModelsService {

    private file: File;

    constructor() { }

    public openFile(file: File) {
        this.file = file;
    }
}