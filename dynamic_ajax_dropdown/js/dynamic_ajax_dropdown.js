(function($) {
Drupal.behaviors.pfe_products_reviews = {
  attach: function (context, settings) {
    
    //Function to get powerreviews configuration settings.
    function get_powerreviews_config_settings() {
      if (typeof Drupal.settings.pfe_products_reviews.powerreiview !== 'undefined') {
        var powerreviews_settings = {
          locale: Drupal.settings.pfe_products_reviews.powerreiview.locale,
          merchant_group_id: Drupal.settings.pfe_products_reviews.powerreiview.merchant_group_id,
          merchant_id: Drupal.settings.pfe_products_reviews.powerreiview.merchant_id,
          api_key: Drupal.settings.pfe_products_reviews.powerreiview.api_key,
        }
        return powerreviews_settings;
      }
    }

    function get_html_class_by_rating(rating) {
      var classname;
      switch (true) {
        case (rating < 0.2):
            classname = 'pr-star-0-filled';
            break;
        case (rating >= 0.2 && rating < 0.4):
            classname = 'pr-star-25-filled';
            break;
        case (rating >= 0.4 && rating < 0.7):
            classname = 'pr-star-50-filled';
            break;
        case (rating >= 0.7 && rating < 0.9):
            classname = 'pr-star-75-filled';
            break;
        case (rating >= 0.9 && rating < 1):
            classname = 'pr-star-100-filled';
            break;
        default:
            classname = 'pr-star-0-filled';
      }
      return classname;
    }
    
    //Function to display rating star, given numeric rating value.
    function get_powerreview_starlayout(rating) {
      var fullstars = parseInt(rating);
      var i;
      var StarHTML = '';
      var diff = rating-fullstars;
      diff = diff.toPrecision(3);
      var delta = parseInt(5-rating);
      for (i = 1; i <= 5; i++) {
        if (i <= fullstars) {
          StarHTML += '<div class="pr-star pr-star-100-filled"></div>';
        }
        else{
          if (diff != 0) {
            var laststar = get_html_class_by_rating(diff);
            StarHTML += '<div class="pr-star '+laststar+'"></div>';
            break;
          }
        }
      }
      if(delta >= 1) {
        for(i=0;i<delta;i++){
          StarHTML += '<div class="pr-star pr-star-0-filled"></div>';
        }
      }
      return StarHTML;
    }

    //Function to display average rating. page_ids should be comma separated value.
    function get_average_rating(page_ids,avg_rating_display_id) {
      var settings = get_powerreviews_config_settings();
      var request = new XMLHttpRequest();
      var url = 'https://readservices-b2c.powerreviews.com/m/%merchant_id/l/%locale/product/%page_ids/snippet?apikey=%api_key';
      var mapObj = {
        '%merchant_id': settings.merchant_id,
        '%locale': settings.locale,
        '%api_key': settings.api_key,
        '%page_ids': page_ids,
      };
      request_url = url.replace(/%merchant_id|%locale|%page_ids|%api_key/gi, function(matched){
        return mapObj[matched];
      });

      request.open("GET", request_url, true);
      request.send();
      request.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          var category = JSON.parse(this.response).results;
          var rating_sum = 0;
          var volume_sum = 0;
          if (category != '') {
            for (x = 0; x < category.length; x++) {
              volume_sum = volume_sum + category[x].rollup.review_count;
              rating_sum = rating_sum + (category[x].rollup.average_rating * category[x].rollup.review_count);
            }
            var rating_avg = rating_sum / volume_sum;
          } else {
            var rating_avg = 0;
          }
          console.log(rating_avg);
          console.log(avg_rating_display_id);
          if ((rating_avg != '' || rating_avg == 0) && avg_rating_display_id) {
            var avr_rating = get_powerreview_starlayout(rating_avg);
            $( avg_rating_display_id ).html(avr_rating);
          }
        }
      };
    }
    
    /******** Write a review form START ********/
    var category_name = $( '.product-category-name' ).val();
    if (category_name == 'none') {
      $( '#product-variant-wrapper' ).hide();
    }
    $('.product-variant-name').once().change(function () {
      renderWriteAReviewForm();
    });
    $('.product-category-name').change(function () {
      var category_name = $( '.product-category-name' ).val();
      if (category_name == 'none') {
        $( '#product-variant-wrapper' ).hide();
      }
      else {
        $( '#product-variant-wrapper' ).show();
      }
      $('#pr-write').empty();
    });
    var renderWriteAReviewForm = function() {
      if (Drupal.settings.pfe_products_reviews.writeareview == '1') {
        if ($('#pr-write').children().length != 0){
          $('#pr-write').empty();
        }
        var page_id_variant = $('.product-variant-name :selected').val();
        var pageid = $('.product-category-name :selected').val();
        if (pageid != 'none') {
          POWERREVIEWS.display.render({
            api_key: Drupal.settings.pfe_products_reviews.powerreiview.api_key,
            locale: Drupal.settings.pfe_products_reviews.powerreiview.locale,
            merchant_group_id: Drupal.settings.pfe_products_reviews.powerreiview.merchant_group_id,
            merchant_id: Drupal.settings.pfe_products_reviews.powerreiview.merchant_id,
            page_id: pageid,
            page_id_variant: page_id_variant,
            on_submit: function(config, data) {
              $('#product-variant-wrapper,.form-item-product-category-name').hide();
            },
            components: {
              Write: 'pr-write'
            }
          });
        }
      }
    }
    renderWriteAReviewForm(); //Display write a riview form on page load.
    /******** Write a review form END ********/
    
    /******** Display Star Rating START ********/
    if (typeof Drupal.settings.pfe_products_reviews.product_category !== 'undefined') {
      var baseUrl = window.location.origin;
      var displayStarRating = function(page_id,snippet_id) {
        var renderRating = {
          locale: Drupal.settings.pfe_products_reviews.powerreiview.locale,
          merchant_group_id: Drupal.settings.pfe_products_reviews.powerreiview.merchant_group_id,
          merchant_id: Drupal.settings.pfe_products_reviews.powerreiview.merchant_id,
          api_key: Drupal.settings.pfe_products_reviews.powerreiview.api_key,
          page_id: page_id,
          review_wrapper_url: baseUrl+'/write-a-review/?pr_page_id='+page_id,
          components: {
            CategorySnippet: snippet_id,
          },
        }
        
        //review display for subcategories.
        if (snippet_id.indexOf('subcat-aggr-') != 0) {
          renderRating.components.ReviewDisplay = 'review-'+snippet_id;
        }
        return renderRating;
      }

      if (typeof POWERREVIEWS !== 'undefined') {
        var viewall_pageids = [];
        var ratings = [];
        $('.pr-rating-category[data-page-id]').each(function(){
          var pageid = $(this).data('page-id');
          if (pageid) {
            viewall_pageids.push(pageid); // page ids for view-all subcat aggregate rating.
            ratings.push(displayStarRating(pageid, pageid));
          }
        });
        
        //To display aggregate rating for subcategories which are in tabs except view-all.
        $('.subcat-aggr-rating[data-subcat-aggr]').each(function(){
          var pageid = $(this).data('subcat-aggr');
          if (pageid) {
            var snippet_id = 'subcat-aggr-' + pageid;
            ratings.push(displayStarRating(pageid, snippet_id));
          }
        });
        POWERREVIEWS.display.render(ratings);

        //To display aggregate rating for subcategories view-all tab.
        if (viewall_pageids && viewall_pageids != '') {
          var all_pageids = viewall_pageids.toString();
          var viewall_cat_rating = get_average_rating(all_pageids,'.subcat-aggr-rating-all-stars');
        }
        
        //To display aggregate starRating for maincategories.
        if (typeof Drupal.settings.pfe_products_reviews.product_category !== 'undefined') {
          if ($('.maincat-aggr-rating-all-stars').length > 0) {
            var subcat_mapping = Drupal.settings.pfe_products_reviews.product_category;
            var all_subcat_pageids = [];
            $.each( subcat_mapping, function( key, value ) {
              all_subcat_pageids.push(key);
            });
            all_subcat_pageids = all_subcat_pageids.toString();
            var maincategories_rating = get_average_rating(all_subcat_pageids,'.maincat-aggr-rating-all-stars');
          }
        }
      }
    }

    /******** Display Star Rating END ********/
  }
};
})(jQuery);
