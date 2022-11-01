
Vue.use(VueToast, {
  position: 'top',
  duration: 2000  
}) ;

const BASE_URL = '../cgi-bin/AppTodo/index.cgi' ; // ruta del cgi

var app = new Vue({
    el: '#app',    
    data: {       
      notes: [],          
      errors: [],      
      text_input: "",
      loaded: false, // carga de la tabla de notas            
      url: BASE_URL,
      method: 'post',        
      max_length: 64, 
      nb_elements: 10, // elementos por página
      paginate_data: [],
      current_page: 1,      
      status_code: 200,      
    },

    methods: { 
        // PAGINACIÓN //
        totalPages() {
          return Math.ceil(this.notes.length / this.nb_elements ) ;          
        },
        
        getDataPage(numberPage) {
          this.current_page = numberPage ;
          this.paginate_data = [] ;
          let ini = (numberPage * this.nb_elements) - this.nb_elements ;
          let fin = (numberPage * this.nb_elements) ;
          this.paginate_data = this.notes.slice(ini,fin) ;           
        },

        getPreviousPage() {
          if (this.current_page > 1) {
            this.current_page-- ;
          }
          this.getDataPage(this.current_page) ;
        },

        getNextPage() {
          if (this.current_page < this.totalPages()){
            this.current_page++ ;
          }
          this.getDataPage(this.current_page) ;
        },

        isActive(numberPage) {
          return (numberPage == this.current_page) ? 'active' : '' ;            
        },

        // FORMULARIO //
        maxLength() { 
          return (this.text_input.length <= this.max_length) ? true : false ;
        },

        required() {
          return (this.text_input) ? true : false ;
        },       

        checkForm() {           
          this.errors.splice(0) ;
          if ( !this.required() ) {                                 
            this.errors.push("Debe ingresar una tarea.") ;              
            return false ;
          }

          if ( !this.maxLength() ) {
            this.errors.push("No puede superar los " + this.max_length + " caracteres.") ;              
            return false ;
          }
          
          return true ;
        }, 
        
        // ACCIONES          
        addNote(){          
          if (this.checkForm() ) {            
            const _this = this ;
            let formData = new FormData() ;
            formData.append('action','CreateNote'); 
            formData.append('note',this.text_input);                                                    
            this.sendPostRequest(formData, function(responseJSON) {              
              if (responseJSON.error_status == false) { //                               
                Vue.$toast.success(responseJSON.message) ;   
                _this.notes.push({ 
                  note: responseJSON.data.note,
                  id: responseJSON.data.id
                }) ;                  
                _this.getDataPage( _this.totalPages() ) ; 
              } else {                
                _this.errors.push( responseJSON.message ) ;                 
              }
            }) ;
          }
          this.text_input = '' ;                                               
        },

        readNotes(){
          let formData = new FormData() ;
          formData.append('action','ReadNotes');            
        },
      
        updateNote(index) {
          const _this = this ;
          let formData = new FormData() ;
          formData.append('action','UpdateNote'); 
          formData.append('id', this.notes[index].id);           
          formData.append('note', this.text_input) ; 

          this.sendPostRequest(formData, function(responseJSON) {
            if (responseJSON.error_status == false) {
              Vue.$toast.info(responseJSON.message) ;
              _this.notes[index].note = responseJSON.data.note ;
              _this.notes[index].id = responseJSON.data.id ;
              _this.getDataPage(_this.current_page) ; //
            } else { 
              _this.errors.splice(0) ;
              _this.errors.push( responseJSON.message ) ;              
            }
          }) ;            
          this.text_input = '' ;       
        },

        deleteNote(index) {             
          const _this = this ;
          let formData = new FormData() ;
          formData.append('action','DeleteNote'); 
          formData.append('id', this.notes[index].id);                      

          this.sendPostRequest(formData, function(responseJSON) {
            if (responseJSON.error_status == false) { 
              Vue.$toast.warning(responseJSON.message) ;                              
              _this.notes.splice(index, 1) ;                            
              _this.getDataPage(_this.current_page > _this.totalPages() ? _this.totalPages() : _this.current_page) ; //
            } else {
              _this.errors.splice(0) ;
              _this.errors.push( responseJSON.message ) ;                                    
            }            
          } ) ;          
        },

        editNote(text) {
          this.text_input = text ;          
        },
          
        getNotes() {
          const noteFormData = new FormData() ;
          noteFormData.append('action','ReadNotes');                       
          const noteParams = new URLSearchParams(noteFormData);
          const _this = this ; 
          this.loaded = true ;
          
          fetch(this.url, {
            method: 'POST',              
            body: noteParams,
          }).then(function(response){            
            _this.status_code = response.status ; // response.ok                        
            return response.json();
          }).then(function(responseJSON){
            if ( _this.status_code == 200 ) {
              _this.notes = responseJSON ;
              _this.getDataPage(1) ;                       
            } else {
              Vue.$toast.error(responseJSON.message) ;                            
            }
            _this.loaded = false ;
          }) ;
              
        },

        sendPostRequest(noteFormData, callbackFunction) {
          const noteParams = new URLSearchParams(noteFormData);                                        
          fetch( BASE_URL, {
            method: 'POST',              
            body: noteParams,
          }).then(function(response){            
            return response.json();
          }).then(function(responseJSON){              
            callbackFunction(responseJSON) ;             
          }) ;
        },                    
      },

      created: function() {
        this.getNotes() ;
      },       
      
      computed: {
        calculatedIndex() {
          return (this.current_page-1) * this.nb_elements ;
        }
      },             
  
  });